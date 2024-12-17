#include "barretenberg/vm2/tracegen_helper.hpp"

#include <array>
#include <functional>
#include <list>

#include "barretenberg/common/thread.hpp"
#include "barretenberg/numeric/bitop/get_msb.hpp"
#include "barretenberg/vm2/tracegen/alu_trace.hpp"
#include "barretenberg/vm2/tracegen/execution_trace.hpp"
#include "barretenberg/vm2/tracegen/precomputed_trace.hpp"
#include "barretenberg/vm2/tracegen/trace_container.hpp"

namespace bb::avm2 {

using namespace bb::avm2::simulation;
using namespace bb::avm2::tracegen;

namespace {

void fill_precomputed_columns(TraceContainer& trace)
{
    PrecomputedTraceBuilder precomputed_builder;
    precomputed_builder.process(trace);
}

} // namespace

TraceContainer AvmTraceGenHelper::generate_trace(EventsContainer&& events)
{
    TraceContainer trace;
    // We process the events in parallel. Ideally the jobs should access disjoint column sets.
    std::array<std::function<void()>, 3> jobs = {
        [&]() {
            // TODO: move parallelism to fill_precomputed_columns when we have more precomputed columns.
            fill_precomputed_columns(trace);
        },
        [&]() {
            ExecutionTraceBuilder exec_builder;
            exec_builder.process(events.execution, events.addressing, trace);
            events.execution.clear();
            events.addressing.clear();
        },
        [&]() {
            AluTraceBuilder alu_builder;
            alu_builder.process(events.alu, trace);
            events.alu.clear();
        },
    };
    parallel_for(jobs.size(), [&](size_t i) { jobs[i](); });

    const auto rows = trace.get_num_rows();
    info("Generated trace with ",
         rows,
         " rows (closest power of 2: ",
         numeric::get_msb(numeric::round_up_power_2(rows)),
         ")");
    return trace;
}

TraceContainer AvmTraceGenHelper::generate_precomputed_columns()
{
    TraceContainer trace;
    fill_precomputed_columns(trace);
    return trace;
}

} // namespace bb::avm2