#ifndef RecoLocalTracker_SiPixelRecHits_PixelCPEFast_h
#define RecoLocalTracker_SiPixelRecHits_PixelCPEFast_h

#include <utility>

#include "CalibTracker/SiPixelESProducers/interface/SiPixelCPEGenericDBErrorParametrization.h"
#include "CondFormats/SiPixelTransient/interface/SiPixelGenError.h"
#include "CondFormats/SiPixelTransient/interface/SiPixelTemplate.h"
#include "HeterogeneousCore/CUDACore/interface/ESProduct.h"
#include "HeterogeneousCore/CUDAUtilities/interface/HostAllocator.h"
#include "RecoLocalTracker/SiPixelRecHits/interface/PixelCPEGenericBase.h"
#include "RecoLocalTracker/SiPixelRecHits/interface/pixelCPEforGPU.h"

class MagneticField;
class PixelCPEFast final : public PixelCPEGenericBase {
public:
  PixelCPEFast(edm::ParameterSet const &conf,
               const MagneticField *,
               const TrackerGeometry &,
               const TrackerTopology &,
               const SiPixelLorentzAngle *,
               const SiPixelGenErrorDBObject *,
               const SiPixelLorentzAngle *);

  ~PixelCPEFast() override = default;

  static void fillPSetDescription(edm::ParameterSetDescription &desc);

  // The return value can only be used safely in kernels launched on
  // the same cudaStream, or after cudaStreamSynchronize.
  const pixelCPEforGPU::ParamsOnGPU *getGPUProductAsync(cudaStream_t cudaStream) const;

  pixelCPEforGPU::ParamsOnGPU const &getCPUProduct() const { return cpuData_; }

private:
  LocalPoint localPosition(DetParam const &theDetParam, ClusterParam &theClusterParam) const override;
  LocalError localError(DetParam const &theDetParam, ClusterParam &theClusterParam) const override;

  void errorFromTemplates(DetParam const &theDetParam, ClusterParamGeneric &theClusterParam, float qclus) const;

  static void collect_edge_charges(ClusterParam &theClusterParam,  //!< input, the cluster
                                   int &q_f_X,                     //!< output, Q first  in X
                                   int &q_l_X,                     //!< output, Q last   in X
                                   int &q_f_Y,                     //!< output, Q first  in Y
                                   int &q_l_Y,                     //!< output, Q last   in Y
                                   bool truncate);

  const float edgeClusterErrorX_;
  const float edgeClusterErrorY_;
  const bool useErrorsFromTemplates_;
  const bool truncatePixelCharge_;

  std::vector<float> xerr_barrel_l1_, yerr_barrel_l1_, xerr_barrel_ln_;
  std::vector<float> yerr_barrel_ln_, xerr_endcap_, yerr_endcap_;
  float xerr_barrel_l1_def_, yerr_barrel_l1_def_, xerr_barrel_ln_def_;
  float yerr_barrel_ln_def_, xerr_endcap_def_, yerr_endcap_def_;

  //--- DB Error Parametrization object, new light templates
  std::vector<SiPixelGenErrorStore> thePixelGenError_;

  // allocate this with posix malloc to be compatible with the cpu workflow
  std::vector<pixelCPEforGPU::DetParams> detParamsGPU_;
  pixelCPEforGPU::CommonParams commonParamsGPU_;
  pixelCPEforGPU::LayerGeometry layerGeometry_;
  pixelCPEforGPU::AverageGeometry averageGeometry_;
  pixelCPEforGPU::ParamsOnGPU cpuData_;

  struct GPUData {
    ~GPUData();
    // not needed if not used on CPU...
    pixelCPEforGPU::ParamsOnGPU paramsOnGPU_h;
    pixelCPEforGPU::ParamsOnGPU *paramsOnGPU_d = nullptr;  // copy of the above on the Device
  };
  cms::cuda::ESProduct<GPUData> gpuData_;

  void fillParamsForGpu();
};

#endif  // RecoLocalTracker_SiPixelRecHits_PixelCPEFast_h
