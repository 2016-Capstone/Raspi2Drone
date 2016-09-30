/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the 
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/
/**
 * @file autoTest.c
 * @brief libARMavlink TestBench automatic
 * @date 11/13/2013
 * @author frederic.dhaeyer@parrot.com
 */

/*****************************************
 *
 *             include file :
 *
 *****************************************/
#include <stdio.h>
#include <stdlib.h>

#include <libARSAL/ARSAL_Print.h>
#include <libARSAL/ARSAL_Thread.h>

#include <string.h>

#include <libARMavlink/libARMavlink.h>

#include "ARMAVLINK_ListUtils.h"
#include "ARMAVLINK_MissionItemUtils.h"

/* ****************************************
 *
 *             define :
 *
 **************************************** */

#define AUTOTEST_TAG "Autotest"

#define AUTOTEST_OUTPUT_FILE_PATH "output.txt"

/* ****************************************
 *
 *           variable declarations :
 *
 **************************************** */
mission_item_list_t *inMissionItemList;

/* ****************************************
 *
 *           function declarations :
 *
 **************************************** */

/**
 * @brief test the utils functions
 */
eARMAVLINK_ERROR testUtils();

/**
 * @brief Create a set of mission items and store them in inMissionItemList
 */
eARMAVLINK_ERROR createTestMissionItems();

/**
 * @brief test the generation of a mavlink file based on the mission items stored in inMissionItemList
 */
eARMAVLINK_ERROR testFileGenerator();
eARMAVLINK_ERROR addMissionItemsToFileGenerator(ARMAVLINK_FileGenerator_t *fileGenerator);

/**
 * @brief test the parsing of the mavlink file written in the function testFileGenerator
 */
eARMAVLINK_ERROR testFileParser();


/*****************************************
 *
 *          implementation :
 *
 *****************************************/

int main(int argc, char *argv[])
{
    eARMAVLINK_ERROR error = ARMAVLINK_OK;
    
    error = testUtils();
    fprintf(stderr, "Utils test : %s\n", ARMAVLINK_Error_ToString(error));
    
    if (error == ARMAVLINK_OK)
    {
        error = createTestMissionItems();
        fprintf(stderr, "Create test mission item : %s\n", ARMAVLINK_Error_ToString(error));
    }
    
    if (error == ARMAVLINK_OK)
    {
        error = testFileGenerator();
        fprintf(stderr, "File Generator test : %s\n", ARMAVLINK_Error_ToString(error));
    }

    if (error == ARMAVLINK_OK)
    {
        error = testFileParser();
        fprintf(stderr, "File Parser test : %s\n", ARMAVLINK_Error_ToString(error));
    }
    
    fprintf(stderr, "Sum up : %s\n", ARMAVLINK_Error_ToString(error));
    
    return 0;
}

eARMAVLINK_ERROR testUtils()
{
    eARMAVLINK_ERROR error = ARMAVLINK_OK;
    
    // create items to add in the list
    mavlink_mission_item_t item1;
    item1.param1            = 0.11f;
    item1.param2            = 0.12f;
    item1.param3            = 0.13f;
    item1.param4            = 0.14f;
    item1.x                 = 0.15f;
    item1.y                 = 0.16f;
    item1.z                 = 0.17f;
    item1.seq               = 11;
    item1.command           = 12;
    item1.target_system     = 13;
    item1.target_component  = 14;
    item1.frame             = 15;
    item1.current           = 16;
    item1.autocontinue      = 17;
    
    mavlink_mission_item_t item2;
    item2.param1            = 0.21f;
    item2.param2            = 0.22f;
    item2.param3            = 0.23f;
    item2.param4            = 0.24f;
    item2.x                 = 0.25f;
    item2.y                 = 0.26f;
    item2.z                 = 0.27f;
    item2.seq               = 21;
    item2.command           = 22;
    item2.target_system     = 23;
    item2.target_component  = 24;
    item2.frame             = 25;
    item2.current           = 26;
    item2.autocontinue      = 27;
    
    mavlink_mission_item_t item3;
    item3.param1            = 0.31f;
    item3.param2            = 0.32f;
    item3.param3            = 0.33f;
    item3.param4            = 0.34f;
    item3.x                 = 0.35f;
    item3.y                 = 0.36f;
    item3.z                 = 0.37f;
    item3.seq               = 31;
    item3.command           = 32;
    item3.target_system     = 33;
    item3.target_component  = 34;
    item3.frame             = 35;
    item3.current           = 36;
    item3.autocontinue      = 37;
    
    mavlink_mission_item_t item4;
    item4.param1            = 0.31f;
    item4.param2            = 0.32f;
    item4.param3            = 0.33f;
    item4.param4            = 0.34f;
    item4.x                 = 0.35f;
    item4.y                 = 0.36f;
    item4.z                 = 0.37f;
    item4.seq               = 31;
    item4.command           = 32;
    item4.target_system     = 33;
    item4.target_component  = 34;
    item4.frame             = 35;
    item4.current           = 36;
    item4.autocontinue      = 37;
    
    mission_item_list_t *list = ARMAVLINK_ListUtils_MissionItemListNew();
    if (list == NULL)
    {
        error = ARMAVLINK_ERROR_ALLOC;
    }
    
    if (error == ARMAVLINK_OK)
    {
        // list = [item1]
        ARMAVLINK_ListUtils_MissionItemListAdd(list, &item1);
        if ((ARMAVLINK_ListUtils_MissionItemListGetSize(list) != 1) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item1, ARMAVLINK_ListUtils_MissionItemListGet(list, 0)) != 1))
        {
            error = ARMAVLINK_ERROR_LIST_UTILS;
        }
    }
    
    if (error == ARMAVLINK_OK)
    {
        // list = [item2, item1]
        ARMAVLINK_ListUtils_MissionItemListInsertMissionItem(list, &item2, 0);
        if ((ARMAVLINK_ListUtils_MissionItemListGetSize(list) != 2) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item2, ARMAVLINK_ListUtils_MissionItemListGet(list, 0)) != 1) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item1, ARMAVLINK_ListUtils_MissionItemListGet(list, 1)) != 1))
        {
            error = ARMAVLINK_ERROR_LIST_UTILS;
        }

    }
    
    if (error == ARMAVLINK_OK)
    {
        // list = [item2, item1, item3]
        ARMAVLINK_ListUtils_MissionItemListInsertMissionItem(list, &item3, 2);
        if ((ARMAVLINK_ListUtils_MissionItemListGetSize(list) != 3) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item2, ARMAVLINK_ListUtils_MissionItemListGet(list, 0)) != 1) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item1, ARMAVLINK_ListUtils_MissionItemListGet(list, 1)) != 1) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item3, ARMAVLINK_ListUtils_MissionItemListGet(list, 2)) != 1))
        {
            error = ARMAVLINK_ERROR_LIST_UTILS;
        }
        
    }
    
    if (error == ARMAVLINK_OK)
    {
        // list = [item2, item1, item4, item3]
        ARMAVLINK_ListUtils_MissionItemListInsertMissionItem(list, &item4, 2);
        if ((ARMAVLINK_ListUtils_MissionItemListGetSize(list) != 4) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item2, ARMAVLINK_ListUtils_MissionItemListGet(list, 0)) != 1) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item1, ARMAVLINK_ListUtils_MissionItemListGet(list, 1)) != 1) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item4, ARMAVLINK_ListUtils_MissionItemListGet(list, 2)) != 1) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item3, ARMAVLINK_ListUtils_MissionItemListGet(list, 3)) != 1))
        {
            error = ARMAVLINK_ERROR_LIST_UTILS;
        }
        
    }
    
    if (error == ARMAVLINK_OK)
    {
        // list = [item2, item4, item3]
        ARMAVLINK_ListUtils_MissionItemListDeleteMissionItem(list, 1);
        if ((ARMAVLINK_ListUtils_MissionItemListGetSize(list) != 3) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item2, ARMAVLINK_ListUtils_MissionItemListGet(list, 0)) != 1) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item4, ARMAVLINK_ListUtils_MissionItemListGet(list, 1)) != 1) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item3, ARMAVLINK_ListUtils_MissionItemListGet(list, 2)) != 1))
        {
            error = ARMAVLINK_ERROR_LIST_UTILS;
        }
        
    }
    
    if (error == ARMAVLINK_OK)
    {
        // list = [item2, item1, item3]
        ARMAVLINK_ListUtils_MissionItemListReplaceMissionItem(list, &item1, 1);
        if ((ARMAVLINK_ListUtils_MissionItemListGetSize(list) != 3) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item2, ARMAVLINK_ListUtils_MissionItemListGet(list, 0)) != 1) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item1, ARMAVLINK_ListUtils_MissionItemListGet(list, 1)) != 1) ||
            (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(&item3, ARMAVLINK_ListUtils_MissionItemListGet(list, 2)) != 1))
        {
            error = ARMAVLINK_ERROR_LIST_UTILS;
        }
        
    }
    
    return error;
}

eARMAVLINK_ERROR createTestMissionItems()
{
    mavlink_mission_item_t missionItem;
    
    eARMAVLINK_ERROR error = ARMAVLINK_OK;
    
    inMissionItemList = ARMAVLINK_ListUtils_MissionItemListNew();
    if (inMissionItemList == NULL)
    {
        error = ARMAVLINK_ERROR_ALLOC;
    }
    
    // missionItem 1
    if (ARMAVLINK_OK == error)
    {
        error = ARMAVLINK_MissionItemUtils_CreateMavlinkNavWaypointMissionItem(&missionItem, 48.879007, 2.367990, 3.0, 0);
        missionItem.seq = 0;
        missionItem.current = 0;
        ARMAVLINK_ListUtils_MissionItemListAdd(inMissionItemList, &missionItem);
    }
    
    // missionItem 2
    if (ARMAVLINK_OK == error)
    {
        error = ARMAVLINK_MissionItemUtils_CreateMavlinkNavWaypointMissionItem(&missionItem, 48.878931, 2.367467, 3.0, 0);
        missionItem.seq = 1;
        ARMAVLINK_ListUtils_MissionItemListAdd(inMissionItemList, &missionItem);
    }
    
    // missionItem 3
    if (ARMAVLINK_OK == error)
    {
        error = ARMAVLINK_MissionItemUtils_CreateMavlinkNavWaypointMissionItem(&missionItem, 48.878800, 2.367518, 3.0, 0);
        missionItem.seq = 2;
        ARMAVLINK_ListUtils_MissionItemListAdd(inMissionItemList, &missionItem);
    }
    
    // missionItem 4
    if (ARMAVLINK_OK == error)
    {
        error = ARMAVLINK_MissionItemUtils_CreateMavlinkNavWaypointMissionItem(&missionItem, 48.878770, 2.368044, 3.0, 0);
        missionItem.seq = 3;
        ARMAVLINK_ListUtils_MissionItemListAdd(inMissionItemList, &missionItem);
    }
    
    // missionItem 5
    if (ARMAVLINK_OK == error)
    {
        error = ARMAVLINK_MissionItemUtils_CreateMavlinkNavWaypointMissionItem(&missionItem, 48.879007, 2.367990, 3.0, 0);
        missionItem.seq = 4;
        ARMAVLINK_ListUtils_MissionItemListAdd(inMissionItemList, &missionItem);
    }
    
    return error;
}

eARMAVLINK_ERROR testFileGenerator()
{
    eARMAVLINK_ERROR error = ARMAVLINK_OK;
    
    ARMAVLINK_Manager_t *managerPtr = ARMAVLINK_Manager_New(&error);
    if(error == ARMAVLINK_OK)
    {
        ARMAVLINK_FileGenerator_t *fileGenerator = ARMAVLINK_FileGenerator_New(&error);
        if(error == ARMAVLINK_OK)
        {
            error = addMissionItemsToFileGenerator(fileGenerator);
            if(error == ARMAVLINK_OK)
            {
                ARMAVLINK_FileGenerator_CreateMavlinkFile(fileGenerator, AUTOTEST_OUTPUT_FILE_PATH);
            }
            
            ARMAVLINK_FileGenerator_Delete(&fileGenerator);
        }
        else
        {
            // if there was an error in creating a new file generator
            ARMAVLINK_FileGenerator_Delete(&fileGenerator);
        }
        
        
        ARMAVLINK_Manager_Delete(&managerPtr);
    }
    else
    {
        // if there was an error in creating a new manager
        ARMAVLINK_Manager_Delete(&managerPtr);
    }

    return error;
}

eARMAVLINK_ERROR addMissionItemsToFileGenerator(ARMAVLINK_FileGenerator_t *fileGenerator)
{
    eARMAVLINK_ERROR error = ARMAVLINK_OK;
    
    mavlink_mission_item_t *missionItem;
    mission_item_list_t *currentList;
    
    int i;
    int nbPointInList = ARMAVLINK_ListUtils_MissionItemListGetSize(inMissionItemList);

    // add all missionItems of inMissionItemList
    for (i = 0; (ARMAVLINK_OK == error) && (i < nbPointInList); i++)
    {
        missionItem = ARMAVLINK_ListUtils_MissionItemListGet(inMissionItemList, i);
        error = ARMAVLINK_FileGenerator_AddMissionItem(fileGenerator, missionItem);
    }
    
    
    if (ARMAVLINK_OK == error)
    {
        currentList = ARMAVLINK_FileGenerator_GetCurrentMissionItemList(fileGenerator, &error);
    }
    
    if (ARMAVLINK_OK == error)
    {
        int nbPointInCurrentList = ARMAVLINK_ListUtils_MissionItemListGetSize(currentList);
        if (nbPointInList == nbPointInCurrentList)
        {
            mavlink_mission_item_t *missionItemInCurrentList;
            for (i = 0; (ARMAVLINK_OK == error) && (i < nbPointInList); i++)
            {
                missionItem = ARMAVLINK_ListUtils_MissionItemListGet(inMissionItemList, i);
                missionItemInCurrentList = ARMAVLINK_ListUtils_MissionItemListGet(currentList, i);
                if (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(missionItem, missionItemInCurrentList) != 1)
                {
                    error = ARMAVLINK_ERROR_LIST_UTILS;
                }
            }
        }
        else
        {
            error = ARMAVLINK_ERROR_LIST_UTILS;
            fprintf(stderr, "nb in list = %i", nbPointInCurrentList);
        }
    }
    
    return error;
}

eARMAVLINK_ERROR testFileParser()
{
    eARMAVLINK_ERROR error = ARMAVLINK_OK;
    
    ARMAVLINK_FileParser_t *fileParser = ARMAVLINK_FileParser_New(&error);
    if(error == ARMAVLINK_OK)
    {
        mission_item_list_t *outMissionItemList = ARMAVLINK_ListUtils_MissionItemListNew();
        if (outMissionItemList == NULL)
        {
            error = ARMAVLINK_ERROR_ALLOC;
        }
        
        if (error == ARMAVLINK_OK)
        {
            error = ARMAVLINK_FileParser_Parse(fileParser, AUTOTEST_OUTPUT_FILE_PATH, outMissionItemList);
        }
        
        if (error == ARMAVLINK_OK)
        {
            int i;
            int nbPointInList = ARMAVLINK_ListUtils_MissionItemListGetSize(outMissionItemList);
            if (nbPointInList != ARMAVLINK_ListUtils_MissionItemListGetSize(inMissionItemList))
            {
                error = ARMAVLINK_ERROR_FILE_PARSER;
                fprintf(stderr, "The list to write and the list to parse do not have the same size\n");
            }
            
            for (i = 0; (ARMAVLINK_OK == error) && (i < nbPointInList); i++)
            {
                mavlink_mission_item_t *outMissionItem = ARMAVLINK_ListUtils_MissionItemListGet(outMissionItemList, i);
                mavlink_mission_item_t *inMissionItem = ARMAVLINK_ListUtils_MissionItemListGet(inMissionItemList, i);
                
                if (ARMAVLINK_MissionItemUtils_MissionItemsAreEquals(inMissionItem, outMissionItem) != 1)
                {
                    error = ARMAVLINK_ERROR_FILE_PARSER;
                    fprintf(stderr, "The mission item %i of the list to write and the parsed are not the same\n", i);
                }
            }
        }
        
        ARMAVLINK_FileParser_Delete(&fileParser);
    }
    else
    {
        // if there was an error in creating a new file parser
        ARMAVLINK_FileParser_Delete(&fileParser);
    }
    
    return error;
}
