/*================================================================================

    csmd/src/daemon/src/csmi_request_handler/CSMINodeAttributesUpdate.cc

  © Copyright IBM Corporation 2015-2018. All Rights Reserved

    This program is licensed under the terms of the Eclipse Public License
    v1.0 as published by the Eclipse Foundation and available at
    http://www.eclipse.org/legal/epl-v10.html

    U.S. Government Users Restricted Rights:  Use, duplication or disclosure
    restricted by GSA ADP Schedule Contract with IBM Corp.

================================================================================*/
/*
* Author: Nick Buonarota
* Email: nbuonar@us.ibm.com
*/

/* Header for this file. */
#include "CSMINodeAttributesUpdate.h"
/* Includes for inventory structs and helper functions for those structs including serialization. */
#include "csmi/include/csmi_type_inv.h"
#include "csmi/include/csmi_type_inv_funct.h"

#include "include/csm_event_type_definitions.h"

#define STATE_NAME "CSMINodeAttributesUpdate:"

// Use this to make changing struct names easier.
#define INPUT_STRUCT csm_node_attributes_update_input_t
#define OUTPUT_STRUCT  csm_node_attributes_update_output_t


bool CSMINodeAttributesUpdate::CreatePayload(
        const std::string& arguments,
        const uint32_t len,
        csm::db::DBReqContent **dbPayload,
        csm::daemon::EventContextHandlerState_sptr ctx )
{
    LOG(csmapi, trace) << STATE_NAME ":CreatePayload: Enter";

    INPUT_STRUCT* input = nullptr;
	
	if( csm_deserialize_struct(INPUT_STRUCT, &input, arguments.c_str(), len) != 0 )
	{
		LOG( csmapi, error ) << STATE_NAME ":CreatePayload: argUnpackFunc failed...";
        LOG( csmapi, trace  ) << STATE_NAME ":CreatePayload: Exit";
        
        ctx->SetErrorCode( CSMERR_MSG_UNPACK_ERROR);
        ctx->SetErrorMessage("Unable to build the query, struct could not be deserialized");
        return false;
	}

	//parameter based helper variables.
	int paramCount = 0;
	bool atLeastOneParameter = false;
	//parameters for attributes that we will allow the user to reset to NULL in CSM DB. 
	bool comment_NULL = false;
	bool feature_1_NULL = false;
	bool feature_2_NULL = false;
	bool feature_3_NULL = false;
	bool feature_4_NULL = false;
	
	std::string stmt = "WITH updated AS ( UPDATE csm_node SET update_time = 'now',";
	
	//helper for keyword compare.
	int keyword_returnCode = 0;
	int keyword_compareCode = 0;
	
	//check to see if we have to do anything for comment. 
	if(input->comment[0] != '\0')
	{
		keyword_returnCode = CAST_stringTools_CSM_KEYWORD_Compare(input->comment, &keyword_compareCode);
		
		if(keyword_returnCode > 0)
		{
			LOG(csmapi, warning) << STATE_NAME ":CreatePayload: CSM_KEYWORD_Compare returned with error code: " << keyword_returnCode;
		}
		
		switch(keyword_compareCode)
		{
			case 2:
				//keyword "#CSM_NULL" was found.
				//this means reset Database field to NULL
				stmt.append("comment = NULL,");
				comment_NULL = true;
				atLeastOneParameter = true;
				break;
			case 0:
				//no match found
				//for now same behavior so fall through
			case 1:
				//keyword found, but no match
				//for now same behavior so fall through
			default:
				//set DB field to whatever user passed in.
				add_param_sql( stmt, input->comment[0],   ++paramCount, "comment=$",   "::text,")
				break;
		}
	}	
	
	//reset helpers back to 0
	keyword_returnCode = 0;
	keyword_compareCode = 0;
	
	//check to see if we have to do anything for feature_1. 
	if(input->feature_1[0] != '\0')
	{
		keyword_returnCode = CAST_stringTools_CSM_KEYWORD_Compare(input->feature_1, &keyword_compareCode);
		
		if(keyword_returnCode > 0)
		{
			LOG(csmapi, warning) << STATE_NAME ":CreatePayload: CSM_KEYWORD_Compare returned with error code: " << keyword_returnCode;
		}
		
		switch(keyword_compareCode)
		{
			case 2:
				//keyword "#CSM_NULL" was found.
				//this means reset Database field to NULL
				stmt.append("feature_1 = NULL,");
				feature_1_NULL = true;
				atLeastOneParameter = true;
				break;
			case 0:
				//no match found
				//for now same behavior so fall through
			case 1:
				//keyword found, but no match
				//for now same behavior so fall through
			default:
				//set DB field to whatever user passed in.
				add_param_sql( stmt, input->feature_1[0], ++paramCount, "feature_1=$", "::text,")
				break;
		}
	}	
	
	//reset helpers back to 0
	keyword_returnCode = 0;
	keyword_compareCode = 0;
	
	//check to see if we have to do anything for feature_2. 
	if(input->feature_2[0] != '\0')
	{
		keyword_returnCode = CAST_stringTools_CSM_KEYWORD_Compare(input->feature_2, &keyword_compareCode);
		
		if(keyword_returnCode > 0)
		{
			LOG(csmapi, warning) << STATE_NAME ":CreatePayload: CSM_KEYWORD_Compare returned with error code: " << keyword_returnCode;
		}
		
		switch(keyword_compareCode)
		{
			case 2:
				//keyword "#CSM_NULL" was found.
				//this means reset Database field to NULL
				stmt.append("feature_2 = NULL,");
				feature_2_NULL = true;
				atLeastOneParameter = true;
				break;
			case 0:
				//no match found
				//for now same behavior so fall through
			case 1:
				//keyword found, but no match
				//for now same behavior so fall through
			default:
				//set DB field to whatever user passed in.
				add_param_sql( stmt, input->feature_2[0], ++paramCount, "feature_2=$", "::text,")
				break;
		}
	}
	
	//reset helpers back to 0
	keyword_returnCode = 0;
	keyword_compareCode = 0;
	
	//check to see if we have to do anything for feature_3. 
	if(input->feature_3[0] != '\0')
	{
		keyword_returnCode = CAST_stringTools_CSM_KEYWORD_Compare(input->feature_3, &keyword_compareCode);
		
		if(keyword_returnCode > 0)
		{
			LOG(csmapi, warning) << STATE_NAME ":CreatePayload: CSM_KEYWORD_Compare returned with error code: " << keyword_returnCode;
		}
		
		switch(keyword_compareCode)
		{
			case 2:
				//keyword "#CSM_NULL" was found.
				//this means reset Database field to NULL
				stmt.append("feature_3 = NULL,");
				feature_3_NULL = true;
				atLeastOneParameter = true;
				break;
			case 0:
				//no match found
				//for now same behavior so fall through
			case 1:
				//keyword found, but no match
				//for now same behavior so fall through
			default:
				//set DB field to whatever user passed in.
				add_param_sql( stmt, input->feature_3[0], ++paramCount, "feature_3=$", "::text,")
				break;
		}
	}
	
	//reset helpers back to 0
	keyword_returnCode = 0;
	keyword_compareCode = 0;
	
	//check to see if we have to do anything for feature_4. 
	if(input->feature_4[0] != '\0')
	{
		keyword_returnCode = CAST_stringTools_CSM_KEYWORD_Compare(input->feature_4, &keyword_compareCode);
		
		if(keyword_returnCode > 0)
		{
			LOG(csmapi, warning) << STATE_NAME ":CreatePayload: CSM_KEYWORD_Compare returned with error code: " << keyword_returnCode;
		}
		
		switch(keyword_compareCode)
		{
			case 2:
				//keyword "#CSM_NULL" was found.
				//this means reset Database field to NULL
				stmt.append("feature_4 = NULL,");
				feature_4_NULL = true;
				atLeastOneParameter = true;
				break;
			case 0:
				//no match found
				//for now same behavior so fall through
			case 1:
				//keyword found, but no match
				//for now same behavior so fall through
			default:
				//set DB field to whatever user passed in.
				add_param_sql( stmt, input->feature_4[0], ++paramCount, "feature_4=$", "::text,")
				break;
		}
	}
	
	
	add_param_sql( stmt, input->state > CSM_NODE_NO_DEF  && input->state < csm_enum_max(csmi_node_state_t), 
		++paramCount, "state=$", "::compute_node_states,")

	add_param_sql( stmt, input->physical_frame_location[0], 
		++paramCount,"physical_frame_location=$", "::text,")
	add_param_sql( stmt, input->physical_u_location[0], 
		++paramCount,"physical_u_location=$", "::text,")
		
	if(paramCount > 0)
	{
		atLeastOneParameter = true;
	}
	
	// Verify the payload.
	if ( atLeastOneParameter )
	{
		// Remove the last comma.
		stmt.back()= ' ';
	}
	else
	{
		LOG(csmapi, error) << STATE_NAME ":CreatePayload: No values supplied";
		ctx->SetErrorCode(CSMERR_MISSING_PARAM);
		ctx->SetErrorMessage("Unable to build update query, no values supplied");
		csm_free_struct_ptr(INPUT_STRUCT, input);

		LOG(csmapi, trace) << STATE_NAME ":CreatePayload: Exit";
		return false;
	}

	// Build the array string.
	std::string array_param = "$";
	array_param.append(std::to_string(++paramCount)).append("::text[]");

	stmt.append("WHERE node_name=ANY(").append(array_param).append( ") RETURNING node_name )"
		"SELECT node FROM unnest (").append(array_param).append( ") as input(node) "
		"LEFT JOIN updated ON (updated.node_name=input.node) "
		"WHERE updated.node_name IS NULL");

	csm::db::DBReqContent *dbReq = new csm::db::DBReqContent( stmt, paramCount );
	if( input->comment[0] ) 
	{
		//make sure its false, otherwise we added it above. 
		if(comment_NULL == false)
		{
			dbReq->AddTextParam(input->comment);
		}
	}
	if( input->feature_1[0] ) 
	{
		//make sure its false, otherwise we added it above. 
		if(feature_1_NULL == false)
		{
			dbReq->AddTextParam(input->feature_1);
		}
	}
	if( input->feature_2[0] ) 
	{
		//make sure its false, otherwise we added it above. 
		if(feature_2_NULL == false)
		{
			dbReq->AddTextParam(input->feature_2);
		}
	}
	if( input->feature_3[0] ) 
	{
		//make sure its false, otherwise we added it above. 
		if(feature_3_NULL == false)
		{
			dbReq->AddTextParam(input->feature_3);
		}
	}
	if( input->feature_4[0] ) 
	{
		//make sure its false, otherwise we added it above. 
		if(feature_4_NULL == false)
		{
			dbReq->AddTextParam(input->feature_4);
		}
	}
	if( input->state && input->state < csm_enum_max(csmi_node_state_t) )
		dbReq->AddTextParam( csm_get_string_from_enum(csmi_node_state_t, input->state));

	if( input->physical_frame_location[0] ) dbReq->AddTextParam(input->physical_frame_location);
	if( input->physical_u_location[0] ) dbReq->AddTextParam(input->physical_u_location);

	dbReq->AddTextArrayParam(input->node_names, input->node_names_count);

	*dbPayload = dbReq;
	csm_free_struct_ptr(INPUT_STRUCT, input);

	LOG(csmapi, debug) << STATE_NAME ":CreatePayload: Parameterized SQL: " << stmt;
    
    
    LOG( csmapi, trace ) << STATE_NAME ":CreatePayload: Exit";

    return true;
}

bool CSMINodeAttributesUpdate::CreateByteArray(
        const std::vector<csm::db::DBTuple *>&tuples,
        char **buf, uint32_t &bufLen,
        csm::daemon::EventContextHandlerState_sptr ctx )
{
    LOG( csmapi, trace ) << STATE_NAME ":CreateByteArray: Enter";

    *buf = nullptr;
    bufLen = 0;

    uint32_t numResults = tuples.size();

    OUTPUT_STRUCT *output = nullptr;
    csm_init_struct_ptr(OUTPUT_STRUCT, output);

    // Set the failure count (0 is success).
    output->failure_count      = numResults;

    if(numResults > 0)
    {
        output->failure_node_names = (char**) malloc( numResults * sizeof(char*) );
        
        for ( uint32_t i = 0; i < numResults; ++i )
        {
            output->failure_node_names[i] = strdup(tuples[i]->data[0]);
        }
    }

    csm_serialize_struct( OUTPUT_STRUCT, output, buf, &bufLen );
    csm_free_struct_ptr( OUTPUT_STRUCT, output );

    LOG( csmapi, trace ) << STATE_NAME ":CreateByteArray: Exit";

    return true;
}

