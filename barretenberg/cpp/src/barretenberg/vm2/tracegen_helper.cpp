#include "barretenberg/vm2/tracegen_helper.hpp"

#include <list>

#include "barretenberg/numeric/bitop/get_msb.hpp"
#include "barretenberg/vm2/tracegen/alu_trace.hpp"
#include "barretenberg/vm2/tracegen/execution_trace.hpp"
#include "barretenberg/vm2/tracegen/trace_container.hpp"

namespace bb::avm2 {

using namespace bb::avm2::simulation;
using namespace bb::avm2::tracegen;

namespace {

void fill_precomputed_columns(TraceContainer& trace)
{
    // Nothing to do yet!
    (void)trace;
}

} // namespace

TraceContainer AvmTraceGenHelper::generate_trace(EventsContainer&& events)
{
    TraceContainer trace;
    fill_precomputed_columns(trace);

    // TODO: We can't parallelize this yet because the TraceContainer is not thread-safe.
    ExecutionTraceBuilder exec_builder;
    AluTraceBuilder alu_builder;
    alu_builder.process(events.alu, trace);
    events.alu.clear();
    exec_builder.process(events.execution, events.addressing, trace);
    events.execution.clear();
    events.addressing.clear();

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