#!/bin/bash
#================================================================================
#   
#    csm_db_clear_archive_time.sh
# 
#    © Copyright IBM Corporation 2015-2018. All Rights Reserved
#
#    This program is licensed under the terms of the Eclipse Public License
#    v1.0 as published by the Eclipse Foundation and available at
#    http://www.eclipse.org/legal/epl-v10.html
#
#    U.S. Government Users Restricted Rights:  Use, duplication or disclosure
#    restricted by GSA ADP Schedule Contract with IBM Corp.
# 
#================================================================================

optstring="ht:n:d:"

TABLES=( csm_allocation_history csm_allocation_node_history csm_allocation_state_history \
    csm_config_history csm_db_schema_version_history csm_diag_result_history csm_diag_run_history \
    csm_dimm_history csm_gpu_history csm_hca_history csm_ib_cable_history csm_lv_history \
    csm_lv_update_history csm_node_history csm_node_state_history csm_processor_socket_history \
    csm_ssd_history csm_ssd_wear_history csm_step_history csm_step_node_history csm_switch_history\
    csm_switch_inventory_history csm_vg_history csm_vg_ssd_history csm_ras_event_action)


DATABASE="csmdb"                            # The database to search for history tables to archive.
db_username="postgres"

while getopts $optstring OPTION
do
    case $OPTION in
        h)
            exit 1;;
        d)
            DATABASE=${OPTARG};;
    esac
done

for table in "${TABLES[@]}"
do
    psql -q -t -U ${db_username} -d ${DATABASE} -c "UPDATE $table SET archive_history_time = NULL;"
done

