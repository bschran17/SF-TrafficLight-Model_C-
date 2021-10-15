#include "pngwriter.hpp"
#include <string>
#include <string.h>
#include <vector>

class Iconmaker
{
public:
  void graphVerticalRoad();
  void graphHorizontalRoad();
  void graphBackslashRoad();
  void graphForwardSlashRoad();
  void createPNG();

private:
  std::vector<std::string> createFileNames();
};