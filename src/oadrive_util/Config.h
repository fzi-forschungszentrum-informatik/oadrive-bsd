// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------
// Copyright (c) 2016, FZI Forschungszentrum Informatik
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!\file
 *
 * \author  Micha Pfeiffer <ueczz@student.kit.edu>
 * \date    2016-02-22
 *
 */
//----------------------------------------------------------------------

#ifndef OADRIVE_UTIL_CONFIG_H
#define OADRIVE_UTIL_CONFIG_H

#include <string>
#include <map>
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>

namespace oadrive {
namespace util {
/*!
   \brief The Config class read the config files (yaml file format).
You have to init this class with the path and the carname.
It search first in the car folder. If there is no file avaible it searchs in the config folder.
 */
class Config
{
  public:
  /*!
       \brief setConfigPath sets the config Path
       \param folder path to the config folder (It must contain a folder with the carname)
       \param carName name of the car (you can have differnt car folders)
       \return
     */
    static std::string setConfigPath( boost::filesystem::path folder, std::string carName );
    /*!
       \brief getDouble returns a double from the config file
       \param category config file name (without .yml)
       \param key name of the config
       \param defaultVal value which is returned if the entry isn't available
       \return
     */
    static double getDouble( std::string category, std::string key, double defaultVal );
    /*!
       \brief getInt returns a Int from the config file
       \param category config file name (without .yml)
       \param key name of the config
       \param defaultVal value which is returned if the entry isn't available
       \return
     */
    static int getInt( std::string category, std::string key, int defaultVal );
    /*!
       \brief getString returns a String from the config file
       \param category config file name (without .yml)
       \param key name of the config
       \param defaultVal value which is returned if the entry isn't available
       \return
     */
    static std::string getString( std::string category, std::string key, std::string defaultVal );
    /*!
       \brief getMat returns a opencv Matrix from the config file
       \param category file name
       \param key name of the config in the file
       \return
     */
    static cv::Mat getMat( std::string category, std::string key );
    /*!
       \brief getBool returns a bool from the config file
       \param category config file name (without .yml)
       \param key name of the config
       \param defaultVal value which is returned if the entry isn't available
       \return
     */
    static bool getBool( std::string category, std::string key, bool defaultVal );

    static boost::filesystem::path getConfigFile( std::string category );
    /*!
       \brief getCarName returns the car name wich is provided  which is provided by setConfigPath()
       \return car name
     */
    static std::string getCarName();
  private:
    Config();   // private constructor -> "static class"

    /*! Holds all values which were already loaded
     * indexing: map[category][key] = value; */
    static std::map< std::string, std::map<std::string, std::string> > mDatabaseString;
    static std::map< std::string, std::map<std::string, double> > mDatabaseDouble;
    static std::map< std::string, std::map<std::string, int> > mDatabaseInt;
    static std::map< std::string, std::map<std::string, cv::Mat> > mDatabaseMat;
    static std::map< std::string, std::map<std::string, bool> > mDatabaseBool;
    static boost::filesystem::path mConfigFolder;
    static boost::filesystem::path mCarConfigFolder;
    static std::string mCarName;
    static bool mInitialized;
};

} // namespace
} // namespace

#endif // OADRIVE_UTIL_CONFIG_H
