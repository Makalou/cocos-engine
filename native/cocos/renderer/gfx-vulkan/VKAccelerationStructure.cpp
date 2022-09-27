#include "VKAccelerationStructure.h"
#include "VKCommands.h"
#include "VKDevice.h"

namespace cc {
namespace gfx {

CCVKAccelerationStructure::CCVKAccelerationStructure() {
    //todo
}

CCVKAccelerationStructure::~CCVKAccelerationStructure() {
    destroy();
}

void CCVKAccelerationStructure::doInit(const AccelerationStructureInfo& info) {
    _gpuAccelerationStructure = ccnew CCVKGPUAccelerationStructure;
    _gpuAccelerationStructure->asGeometries = _info.geomtries;
    //todo
 
    cmdFuncCCVKCreateAcclerationStructure(CCVKDevice::getInstance(), _gpuAccelerationStructure); 
}

void CCVKAccelerationStructure::doUpdate() {

      _gpuAccelerationStructure->asGeometries = _info.geomtries;

    auto* device = CCVKDevice::getInstance();
    auto* cmdBuf = device->getCommandBuffer();

    cmdBuf->begin();
    cmdBuf->updateAccelerationStructure(this);
    cmdBuf->end();

    device->flushCommands(&cmdBuf, 1);
    device->getQueue()->submit(&cmdBuf, 1);
}

void CCVKAccelerationStructure::doBuild() {

     _gpuAccelerationStructure->asGeometries = _info.geomtries;

    auto* device = CCVKDevice::getInstance();
    auto* cmdBuf = device->getCommandBuffer();

    cmdBuf->begin();
    cmdBuf->buildAccelerationStructure(this);
    cmdBuf->end();

    device->flushCommands(&cmdBuf, 1);
    device->getQueue()->submit(&cmdBuf, 1);
}

void CCVKAccelerationStructure::doDestroy() {
    if (_gpuAccelerationStructure) {
        //CCVKDevice::getInstance()->gpuRecycleBin()->collect(_gpuAccelerationStructure);
        VkDevice device = CCVKDevice::getInstance()->gpuDevice()->vkDevice;
        vkDestroyAccelerationStructureKHR(device, _gpuAccelerationStructure->vkAccelerationStructure,nullptr);
        _gpuAccelerationStructure->accelStructBuffer->destroy();
        if (_gpuAccelerationStructure->instancesBuffer != nullptr)
            _gpuAccelerationStructure->instancesBuffer->destroy();
        _gpuAccelerationStructure = nullptr;
    }
    //todo gpuRecycleBin Implementation
}

} // namespace gfx
} //namespace cc
