#include "console.h"
#include "logger.h"
#include "XGetOpt.h"

#include "ComGeo.h"
#include <QtWidgets/QApplication>

#include <algorithm>
#include <sstream>

void showVersion(const char*);
void showHelp(const char*);
bool isInteger(const char*);

static const uint8_t MAJOR = 1;
static const uint8_t MINOR = 0;
static const uint8_t PATCH = 0;

static const char* name = "ComGeo";


/**********************************************************************************************************************
 * Function: main
 *
 * Abstract:
 *
 * Input   :
 *
 * Returns :
 *
 * Written : Aug 2019 (gkhuber)
 *           Nov 2025 - added support for logger functionality.
 *           Dec 2025 - added support for command line arguments
 *********************************************************************************************************************/
int main(int argc, char *argv[])
{
  uint32_t nRet;
  uint32_t imageWidth = 1024;
  uint32_t imageHeight = 768;
  uint32_t scale = 10;
  int32_t dbgLevel = CLogger::level::WARNING;
  int      choice = -1;


  allocConsole();
  
  
  while (-1 != (choice = getopt(argc, argv, "s:S:dvh")))
  {
    switch (choice)
    {
      case 's':           // set image size widthxlength
      {
        uint32_t width = 0, height = 0;
        char     del;
        std::stringstream  ss(optarg);
        ss >> width >> del >>  height;

        if ((width == 0) || (height == 0))
        {
          std::cout << "malformed size string: " << optarg << " using default size of " << imageWidth << "x" << imageHeight << std::endl;
        }
        else
        {
          imageWidth = width;
          imageHeight = height;

          std::cout << "setting image dimensions to " << imageWidth << "x" << imageHeight << std::endl;
        }
        

      }
      break;

      case 'S':           // set image scale # pixels = 1 unit
        if (isInteger(optarg))
          scale = atoi(optarg);
        else
        {
          std::cout << "The value for scale must be an integer, you entered: " << optarg;
          std::cout << " using the default scale of 1 uint = " << scale << " pixels" << std::endl;
        }

        if ((scale < 1) || (1000 < scale))
        {
          std::cout << "scale must be a value in the range of 1 - 1000" << 
            " using the default scale of 1 uint = " << scale << " pixels" << std::endl;
        }
      break;

      case 'v':           // version of the program
        showVersion(name);
        exit(0);
        break;

      case 'd':           // adjust debug level, lower values more infomation
        --dbgLevel;
        if (dbgLevel < 0) dbgLevel = 0;
        break;

      case '?':           // unknown option
        std::cout << argv[optind] << " is an unrecognized command line option" << std::endl;
        [[fallthrough]];
      case 'h':           // help screen
        showHelp(name);
        exit(0);
        break;

    }
  }

  QApplication a(argc, argv);


  CLogger* pLogger = CLogger::getInstance();
  pLogger->setLevel(dbgLevel);
  pLogger->regOutDevice(0, cmdColorOut);

  ComGeo w(imageWidth, imageHeight, scale);      
  w.show();
  nRet = a.exec();

  deallocConsole();
  pLogger->CLogger::delInstance();


  return nRet;
}

bool isInteger(const char* str)
{
  const char* ep = str + strlen(str);

  return ep == std::find_if(str, ep, [](const char ch)->bool { return !isdigit(ch); });
}

void showVersion(const char* name)
{
  std::cout << name << " a computational geometry exploration tool" << std::endl;
  std::cout << "version: " << static_cast<uint32_t>(MAJOR) << "." << static_cast<uint32_t>(MINOR) << "." << static_cast<uint32_t>(PATCH) << std::endl;
}
//"s:S:dvh"
void showHelp(const char* name)
{
  std::cout << name << " a computational geometry exploration tool" << std::endl;
  std::cout << "usage: " << name << "[options]\n\noptions are:" << std::endl;
  std::cout << "s <size>     set the image size to <size>.  <size> should be in the format width x height.  default is 1024x768." << std::endl;
  std::cout << "S <scale>    sets the image scale to <scale>.  <scale> should be an integer in the range of 1 to 1000." << std::endl;
  std::cout << "             default value is 10 pixels = 1 unit" << std::endl;
  std::cout << "d            increases the verbosity of debug output messages, may be use multiple times" << std::endl;
  std::cout << "v            shows version information and then exits" << std::endl;
  std::cout << "h            shows a brief help screen (this screen) and then exits" << std::endl;
}
