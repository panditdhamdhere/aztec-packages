#include "barretenberg/vm2/tracegen_helper.hpp"

#include <list>

#include "barretenberg/vm2/tracegen/alu_trace_builder.hpp"
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
    AluTraceBuilder alu_builder;
    alu_builder.process(events.alu, trace);
    events.alu.clear();

    return trace;
}

TraceContainer AvmTraceGenHelper::generate_precomputed_columns()
{
    TraceContainer trace;
    fill_precomputed_columns(trace);
    return trace;
}

} // namespace bb::avm2