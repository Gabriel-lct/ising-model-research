#include "utils.hpp"
#include <iostream>
#include <sys/stat.h>
#include "H5Cpp.h"

using namespace H5;

void show_configuration(const configuration &config)
{
  int X = config.size();
  int Y = config[0].size();

  std::cout << "Configuration :" << std::endl;
  for (int i = 0; i < X; ++i)
  {
    for (int j = 0; j < Y; ++j)
    {
      std::cout << config[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

bool save_states(const std::vector<configuration> &steps, const std::string filename, double J, double T, int N, int nb_intermediate_config)
{
  try
  {

    // Create the file with HDF5
    H5File file(filename, H5F_ACC_TRUNC);

    if (steps.empty())
    {
      std::cerr << "Nothing to save, steps is empty" << std::endl;
      return false;
    }

    int X = steps[0].size();
    int Y = steps[0][0].size();

    // Save the metadata
    /// Magical scalar space
    DataSpace scalar_space(H5S_SCALAR);
    /// Create the metadata group inside the file
    Group metadata = file.createGroup("/metadata");
    /// Save each metadata at a time
    Attribute attr_J = metadata.createAttribute("J", PredType::NATIVE_DOUBLE, scalar_space);
    attr_J.write(PredType::NATIVE_DOUBLE, &J);

    Attribute attr_T = metadata.createAttribute("T", PredType::NATIVE_DOUBLE, scalar_space);
    attr_T.write(PredType::NATIVE_DOUBLE, &T);

    Attribute attr_N = metadata.createAttribute("N", PredType::NATIVE_INT, scalar_space);
    attr_N.write(PredType::NATIVE_INT, &N);

    Attribute attr_X = metadata.createAttribute("X", PredType::NATIVE_INT, scalar_space);
    attr_X.write(PredType::NATIVE_INT, &X);

    Attribute attr_Y = metadata.createAttribute("Y", PredType::NATIVE_INT, scalar_space);
    attr_Y.write(PredType::NATIVE_INT, &Y);

    int step_size = N / nb_intermediate_config;
    Attribute attr_step_size = metadata.createAttribute("step_size", PredType::NATIVE_INT, scalar_space);
    attr_step_size.write(PredType::NATIVE_INT, &step_size);

    // Save each configuration
    hsize_t dims[3] = {
        static_cast<hsize_t>(nb_intermediate_config),
        static_cast<hsize_t>(X),
        static_cast<hsize_t>(Y),
    };
    DataSpace dataspace(3, dims);

    DataSet dataset = file.createDataSet("/configurations", PredType::NATIVE_INT8, dataspace);

    /// Flatten data to save in a H5 format
    std::vector<int8_t> data_flat(nb_intermediate_config * X * Y);
    int idx = 0;

    for (const configuration &config : steps)
    {
      for (const auto &row : config)
      {
        for (const auto &value : row)
        {
          data_flat[idx++] = static_cast<int8_t>(value);
        }
      }
    }
    dataset.write(data_flat.data(), PredType::NATIVE_INT8);

    file.close();

    std::cout << "✓ HDF5 backup completed : " << filename << std::endl;
    std::cout << "  " << nb_intermediate_config << " states of " << X << "x" << Y << std::endl;

    return true;
  }
  catch (FileIException &error)
  {
    error.printErrorStack();
    return false;
  }
  catch (DataSetIException &error)
  {
    error.printErrorStack();
    return false;
  }
  catch (DataSpaceIException &error)
  {
    error.printErrorStack();
    return false;
  }
}