#include "iconmaker.hpp"

void graphVerticalRoad(int red, int green, int blue, pngwriter p)
{
  p.line(30, 1, 30, 64, red, green, blue);
  p.line(31, 1, 31, 64, red, green, blue);
  p.line(32, 1, 32, 64, red, green, blue);
  p.line(33, 1, 33, 64, red, green, blue);
  p.line(34, 1, 34, 64, red, green, blue);
  p.line(35, 1, 35, 64, red, green, blue);
  p.close();
}

void graphHorizontalRoad(int red, int green, int blue, pngwriter p)
{
  p.line(1, 30, 64, 30, red, green, blue);
  p.line(1, 31, 64, 31, red, green, blue);
  p.line(1, 32, 64, 32, red, green, blue);
  p.line(1, 33, 64, 33, red, green, blue);
  p.line(1, 34, 64, 34, red, green, blue);
  p.line(1, 35, 64, 35, red, green, blue);
  p.close();
}

void graphForwardSlashRoad(int red, int green, int blue, pngwriter p)
{
  int j = 1;
  for (int i = 1; i < 65; i++)
  {
    if (i - 3 >= 1)
    {
      p.plot(i - 3, j, red, green, blue);
    }
    if (i - 2 >= 1)
    {
      p.plot(i - 2, j, red, green, blue);
    }
    if (i - 1 >= 1)
    {
      p.plot(i - 1, j, red, green, blue);
    }
    p.plot(i, j, red, green, blue);
    if (i + 1 <= 64)
    {
      p.plot(i + 1, j, red, green, blue);
    }
    if (i + 2 <= 64)
    {
      p.plot(i + 2, j, red, green, blue);
    }
    if (i + 3 <= 64)
    {
      p.plot(i + 3, j, red, green, blue);
    }
    j = j + 1;
  }
  p.close();
}

void graphBackSlashRoad(int red, int green, int blue, pngwriter p)
{
  int j = 1;
  for (int i = 64; i >= 1; i--)
  {
    if (i - 3 >= 1)
    {
      p.plot(i - 3, j, red, green, blue);
    }
    if (i - 2 >= 1)
    {
      p.plot(i - 2, j, red, green, blue);
    }
    if (i - 1 >= 1)
    {
      p.plot(i - 1, j, red, green, blue);
    }
    p.plot(i, j, red, green, blue);
    if (i + 1 <= 64)
    {
      p.plot(i + 1, j, red, green, blue);
    }
    if (i + 2 <= 64)
    {
      p.plot(i + 2, j, red, green, blue);
    }
    if (i + 3 <= 64)
    {
      p.plot(i + 3, j, red, green, blue);
    }
    j = j + 1;
  }
  p.close();
}

std::vector<std::string> createFileNames()
{
  std::vector<std::string> fileNames;
  std::string fileName = "";
  std::string w = "";
  std::string x = "";
  std::string y = "";
  std::string z = "";
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      switch (i)
      {
      case 0:
        w = "r";
        break;
      case 1:
        w = "g";
        break;
      case 2:
        w = "y";
        break;
      }
      switch (j)
      {
      case 0:
        x = "r";
        break;
      case 1:
        x = "g";
        break;
      case 2:
        x = "y";
        break;
      }
      fileName = "i2" + w + x + ".png";
      fileNames.push_back(fileName);
      for (int k = 0; k < 3; k++)
      {
        switch (k)
        {
        case 0:
          y = "r";
          break;
        case 1:
          y = "g";
          break;
        case 2:
          y = "y";
          break;
        }
        fileName = "i3" + w + x + y + ".png";
        fileNames.push_back(fileName);
        for (int l = 0; l < 3; l++)
        {
          switch (l)
          {
          case 0:
            z = "r";
            break;
          case 1:
            z = "g";
            break;
          case 2:
            z = "y";
            break;
          }
          fileName = "i4" + w + x + y + z + ".png";
          fileNames.push_back(fileName);
        }
      }
    }
  }
  return fileNames;
}

void createPNG(std::string fileName)
{
  const char *c = fileName.c_str();
  pngwriter p = pngwriter(64, 64, 65535, c);
  char secondChar = fileName[1];
  int numRoads = secondChar - '0';
  if (numRoads == 2)
  {
    char color1 = fileName[2];
    char color2 = fileName[3];
    switch (color1)
    {
    case 'r':
      graphHorizontalRoad(65535, 0, 0, p);
      break;
    case 'g':
      graphHorizontalRoad(0, 65535, 0, p);
      break;
    case 'y':
      graphHorizontalRoad(65535, 65535, 0, p);
      break;
    }
    p.readfromfile(c);
    switch (color2)
    {
    case 'r':
      graphVerticalRoad(65535, 0, 0, p);
      break;
    case 'g':
      graphVerticalRoad(0, 65535, 0, p);
      break;
    case 'y':
      graphVerticalRoad(65535, 65535, 0, p);
      break;
    }
  }
  else if (numRoads == 3)
  {
    char color1 = fileName[2];
    char color2 = fileName[3];
    char color3 = fileName[4];
    switch (color1)
    {
    case 'r':
      graphHorizontalRoad(65535, 0, 0, p);
      break;
    case 'g':
      graphHorizontalRoad(0, 65535, 0, p);
      break;
    case 'y':
      graphHorizontalRoad(65535, 65535, 0, p);
      break;
    }
    p.readfromfile(c);
    switch (color2)
    {
    case 'r':
      graphBackSlashRoad(65535, 0, 0, p);
      break;
    case 'g':
      graphBackSlashRoad(0, 65535, 0, p);
      break;
    case 'y':
      graphBackSlashRoad(65535, 65535, 0, p);
      break;
    }
    p.readfromfile(c);
    switch (color3)
    {
    case 'r':
      graphForwardSlashRoad(65535, 0, 0, p);
      break;
    case 'g':
      graphForwardSlashRoad(0, 65535, 0, p);
      break;
    case 'y':
      graphForwardSlashRoad(65535, 65535, 0, p);
      break;
    }
  }
  else
  {
    char color1 = fileName[2];
    char color2 = fileName[3];
    char color3 = fileName[4];
    char color4 = fileName[5];
    switch (color1)
    {
    case 'r':
      graphHorizontalRoad(65535, 0, 0, p);
      break;
    case 'g':
      graphHorizontalRoad(0, 65535, 0, p);
      break;
    case 'y':
      graphHorizontalRoad(65535, 65535, 0, p);
      break;
    }
    p.readfromfile(c);
    switch (color2)
    {
    case 'r':
      graphBackSlashRoad(65535, 0, 0, p);
      break;
    case 'g':
      graphBackSlashRoad(0, 65535, 0, p);
      break;
    case 'y':
      graphBackSlashRoad(65535, 65535, 0, p);
      break;
    }
    p.readfromfile(c);
    switch (color3)
    {
    case 'r':
      graphVerticalRoad(65535, 0, 0, p);
      break;
    case 'g':
      graphVerticalRoad(0, 65535, 0, p);
      break;
    case 'y':
      graphVerticalRoad(65535, 65535, 0, p);
      break;
    }
    p.readfromfile(c);
    switch (color4)
    {
    case 'r':
      graphForwardSlashRoad(65535, 0, 0, p);
      break;
    case 'g':
      graphForwardSlashRoad(0, 65535, 0, p);
      break;
    case 'y':
      graphForwardSlashRoad(65535, 65535, 0, p);
      break;
    }
  }
}

int main()
{
  std::string directory = "files";
  chdir(directory.c_str());
  std::vector<std::string> fileNames = createFileNames();
  for (std::string fileName : fileNames)
  {
    createPNG(fileName);
  }

  return 0;
}