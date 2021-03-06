/*******************************************************************************
 |    util.h
 |
 |  © Copyright IBM Corporation 2015,2016. All Rights Reserved
 |
 |    This program is licensed under the terms of the Eclipse Public License
 |    v1.0 as published by the Eclipse Foundation and available at
 |    http://www.eclipse.org/legal/epl-v10.html
 |
 |    U.S. Government Users Restricted Rights:  Use, duplication or disclosure
 |    restricted by GSA ADP Schedule Contract with IBM Corp.
 *******************************************************************************/

#ifndef COMMON_UTIL_H
#define COMMON_UTIL_H

#include <chrono>
#include <exception>
#include <iostream>
#include <string>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <sys/types.h>

#include "Uuid.h"

namespace pt = boost::property_tree;

class Timer
{
public:
    Timer()
    {
        reset();
    }

    inline void forcePop()
    {
        forcePopNextTime = true;

        return;
    }

    inline double getCurrentElapsedTimeInterval()
    {
        return elapsedTime();
    }

    inline bool isSnoozing()
    {
        return snoozing;
    }

    inline int popped(const double pTimeInterval)
    {
        int l_RC = 0;

        double l_Temp = elapsedTime();
        if (forcePopNextTime || l_Temp > pTimeInterval)
        {
            reset();
            l_RC = 1;
        }
//        printf("Timer::popped(): TimeInterval = %f, ElapsedTime = %f, rc=%d\n", pTimeInterval, l_Temp, l_RC);

        return l_RC;
    }

    inline void reset()
    {
        forcePopNextTime = false;
        snoozing = false;
        starttime = myclock::now();

        return;
    }

    inline void setSnooze(const bool pValue=true)
    {
        snoozing = pValue;

        return;
    }

    double elapsedTime() const
    {
        return std::chrono::duration_cast<secs> (myclock::now() - starttime).count();
    }

private:
    typedef std::chrono::high_resolution_clock myclock;
    typedef std::chrono::duration<double, std::ratio<1> > secs;
    volatile bool forcePopNextTime;
    volatile bool snoozing;
    std::chrono::time_point<myclock> starttime;
};

extern void copyStat2attr_out(const struct stat& s, struct fuse_attr* attr);

/**
    \brief Find the device name given a logical volume uuid

    \todo URL format should be more flexible, including hostname conversion
    \param[in]  pUuid Character array representation of a logical volumne uuid
    \param[out] pDevName Device name
    \param[out] pLength  Available length for device name
 */
extern void getLogicalVolumeDevName(const Uuid& uuid, char* pDevName, size_t pLength);

/**
    \brief Parses a URL into an ipaddress and port

    \todo URL format should be more flexible, including hostname conversion
    \param[in]  pUrl x.x.x.x:port format only
    \param[out] pIPAddress ipaddress suitable for inet_pton()
    \param[out] pPort port number
    \return Error code
    \retval 0 Success
    \retval -1 Failure
 */
extern int getIPPort(std::string pUrl, std::string& pIPAddress, uint16_t& pPort);

/**
    \brief Returns an ipaddress for a specific interface

    \todo URL format should be more flexible, including hostname conversion
    \param[in]  pInterface Interface to use
    \param[out] pIPAddress ipaddress suitable for inet_pton()
    \return Error code
    \retval 0 Success
    \retval -1 Failure
 */
extern int getIPAddrByInterface(const std::string& pInterface, std::string& pIPAddress);

/**
    \brief Returns the logical volume device name for the input file name

    \param[in]  pFile File name
    \param[out] pLogicalVolumeDeviceName Logical volume device name
    \return Error code
    \retval 0 Success
    \retval -1 Failure
 */
extern int getLogicalVolumeDeviceName(const std::string pFile, char* &pLogicalVolumeDeviceName);

const std::string CORAL_LVM_BREADCRUMB = ".coral_lvm";

/**
    \brief Returns the uuid for a logical volume device name

    \param[in]  pLogicalVolumeDeviceName Logical volume device name
    \param[out] pUuid uuid for logical volume
    \return Error code
    \retval 0 Success
    \retval -1 Failure
 */
extern int getUUID(const char* pLogicalVolumeDeviceName, Uuid& pUuid);

/**
    \brief Returns the logical volume uuid for the input file name

    \param[in]  pFile File name
    \param[out] pUuid uuid for logical volume of the input file
    \return Error code
    \retval 0 Success
    \retval -1 Failure
 */
extern int getLogicalVolumeUUID(const std::string& pFile, Uuid& pUuid);

/**
    \brief Returns the logical volume uuid for the path of the input file name

    \param[in]  pFile File name
    \param[out] pUuid uuid for logical volume of the input file
    \return Error code
    \retval 0 Success
    \retval -1 Failure
 */
extern int getLogicalVolumeUUIDForPath(const std::string& pFile, Uuid& pUuid);


class Config {
  public:
    Config() {
        loaded = false;
        path.clear();
    }

    inline int load(const std::string pConfigFile)
    {
        int rc = 0;
        if (!loaded)
        {
            try
            {
                pt::read_json(pConfigFile.c_str(), tree);
                char* l_Path = 0;
                l_Path= realpath(pConfigFile.c_str(), NULL);
                if (l_Path)
                {
                    path.assign(l_Path);
                    loaded = true;
                    free(l_Path);
                }
            }
            catch (std::exception& e)
            {
                std::cerr << "Error: " << e.what() << "\n";
                rc = -2;
            }
        } else {
            std::cerr << "Configuration has already been loaded from " << path << "\n";
            rc = -1;
        }

        return rc;
    }

    inline std::string getPath() { return path; };

    inline pt::ptree getTree() { return tree; };

    inline int isLoaded() { return loaded; };

    pt::ptree   tree;
    bool        loaded;
    std::string path;
};

// NOTE: The following two symbols are exported for
//       txp::Msg which is currently build as a static
//       library.
extern Config curConfig;
extern pt::ptree config;

/**
    \brief Is Device Mounted

    \param[in]  pDevName
    \param[out] pMountPoint - Returned mount point (can be NULL)
    \param[in}  pMountPointLength - Length of space addressed by pMountPoint
    \return Return code
    \retval 0 Not mounted
    \retval 1 Mounted
 */
int isDeviceMounted(char* pDevName, char* &pMountPoint, size_t pMountPointLength);

/**
    \brief Is File System Mounted

    \param[in]  pPath Path for file system mount point
    \return Return code
    \retval 0 Not mounted
    \retval 1 Mounted
 */
int isFileSystemMounted(char* pPath);

/**
    \brief Leave LVM Breadcrumb

    \param[in]  pMountPoint Path for file system mount point
    \param[in]  pUuid UUID for the logical volume
    \param[in]  pDevName Device name for the logical volume
    \return Return code
    \retval 0 Success
    \retval 1 Failure
 */
int leaveLVM_BreadCrumb(const char* pMountPoint, const Uuid& pUuid, const char* pDevName);

/**
    \brief Retrieve LVM Breadcrumb

    \param[in]  pMountPoint Path for file system mount point
    \param[out] pUuid UUID for the logical volume
    \param[out] pDevName Device name for the logical volume
    \return Return code
    \retval 0 Success
    \retval 1 Failure
 */
int retrieveLVM_BreadCrumb(const std::string pMountPoint, Uuid* pUuid, std::string* pDevName);

/**
    \brief Print arguments to a program

    \param[in] pArgc Number of arguments
    \param[in] pArgv Array of arguments
 */
inline void printargs(int pArgc, char *pArgv[]) {
    printf("\nNumber of argv is argc=%u\n", pArgc);
    for (int i=0; i<pArgc; i++){
        printf("argv[%u]=%s \n", i, pArgv[i]);
    }
}

/**
    \brief strCpy Performs a copy of a character array
    \note  Copied data will be null terminated so the pTarger/pSize
           specifidations should account for this null character.

    \param[out] pTarget Data is copied to this address
    \param[in]  pSource Data is copied from this address
    \param[in]  pSize Size of data to copy
 */
inline void strCpy(char* pTarget, const char* pSource, size_t pSize) {
#ifdef strlcpy
    strlcpy(pTarget, pSource, pSize);
#else
    strncpy(pTarget, pSource, pSize-1);
    pTarget[pSize-1] = 0;
#endif
    return;
}


extern std::vector<std::string> buildTokens(const std::string& str, const char* sep);
extern std::vector<std::string> runCommand(const std::string& cmd, bool flatfile = false,bool noException=true);
extern int fuzzyMatch(const std::string& pLine, std::vector<std::string>& pSubStrings);

#endif // COMMON_UTIL_H

