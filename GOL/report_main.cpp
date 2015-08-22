#include <iostream>
#include <cstdlib>
#include <cstring>

#include "serial.h"
#include "simple_cuda.cuh"

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    std::cout << "Usage : " << argv[0]
    << "\n\t inputGoLGridFile \n\t N(gridSize) \n\t numberOfGenerations"<< std::endl;
    return -1;
  }

  int N = atoi(argv[2]);
  int maxGen = atoi(argv[3]);
  bool* startingGrid = new bool[N * N];
  if (startingGrid == NULL)
  {
    std::cout << "Could not allocate memory for the initial grid!" << std::endl;
    return -1;
  }
  bool* finalSerialGrid = new bool[N * N];
  if (finalSerialGrid == NULL)
  {
    std::cout << "Could not allocate memory for the final grid for the serial code!" << std::endl;
    delete[] startingGrid;
    return -1;
  }

  if (!utilities::readFile(startingGrid, argv[1], N))
  {
    std::cout << "Could not read input file!" << std::endl;
    return -1;
  }

  bool* serialStartingGrid = new bool[N * N];
  if (serialStartingGrid == NULL)
  {
    std::cout << "Could not allocate memory for the initial grid of the serial code!" << std::endl;
  }
  memcpy(serialStartingGrid, startingGrid, N * N * sizeof(bool));
  serial::execSerial(serialStartingGrid, finalSerialGrid, N, maxGen);

  bool* simpleGpuStartingGrid = new bool[N * N];
  bool* simpleGpuFinalGrid = new bool[N * N];
  if (simpleGpuStartingGrid == NULL)
  {
    std::cout << "Could not allocate memory for the initial grid array(simple gpu version)!" << std::endl;
    return -1;
  }
  if (simpleGpuFinalGrid == NULL)
  {
    std::cout << "Could not allocate memory for the final grid array(simple gpu version)!" << std::endl;
    return -1;
  }
  memcpy(simpleGpuStartingGrid, startingGrid, N * N * sizeof(bool));
  simple_cuda(simpleGpuStartingGrid, simpleGpuFinalGrid, N, maxGen);


  delete[] startingGrid;
  delete[] serialStartingGrid;
  delete[] finalSerialGrid;
  delete[] simpleGpuStartingGrid;
  return 0;
}
