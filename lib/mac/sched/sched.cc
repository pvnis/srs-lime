
#include "sched.h"

using namespace srsgnb;

sched::sched(sched_cfg_notifier& notifier) :
  mac_notifier(notifier), logger(srslog::fetch_basic_logger("MAC")), pending_events(ue_db, cells)
{}

bool sched::handle_cell_configuration_request(const cell_configuration_request_message& msg)
{
  pending_events.handle(msg);
  return true;
}

void sched::handle_rach_indication(const rach_indication_message& msg)
{
  pending_events.handle(msg);
}

const dl_sched_result* sched::get_dl_sched(slot_point sl, du_cell_index_t cell_index)
{
  slot_indication(sl, cell_index);

  return cells[cell_index]->get_dl_sched(sl);
}

const ul_sched_result* sched::get_ul_sched(slot_point sl, du_cell_index_t cell_index)
{
  // TODO
  return cells[cell_index]->get_ul_sched(sl);
}

void sched::slot_indication(slot_point sl_tx, du_cell_index_t cell_index)
{
  srsran_sanity_check(cell_index < cells.size(), "Invalid cell index");
  auto& cell = cells[cell_index];

  // 1. Reset cell resource grid state.
  cell->slot_indication(sl_tx);

  // 2. Process pending events.
  pending_events.run(sl_tx, cell_index);

  cell_resource_allocator res_alloc{cell->res_grid_pool};

  // 3. Schedule DL signalling.
  // TODO

  // 4. Schedule RARs and Msg3.
  cell->ra_sch.run_slot(res_alloc);

  // 5. Schedule UE DL and UL data.
  // TODO
}
