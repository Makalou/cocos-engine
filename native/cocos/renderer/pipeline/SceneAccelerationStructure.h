#pragma once

#include "renderer/gfx-base/GFXAccelerationStructure.h"
#include "base/Ptr.h"
#include "base/std/container/unordered_set.h"

namespace cc
{

namespace scene{
class RenderScene;
class Model;
}
namespace pipeline
{
    class GlobalDSManager;
    class SceneAccelerationStructure final
    {
    public:
        SceneAccelerationStructure();
        void update(const scene::RenderScene* scene);
        inline void handleNewModel(const IntrusivePtr<scene::Model>& model);
        inline void handleModel(const IntrusivePtr<scene::Model>& model);
        void destroy();
    protected:
        IntrusivePtr<gfx::AccelerationStructure> _topLevelAccelerationStructure;
        ccstd::vector<IntrusivePtr<gfx::AccelerationStructure>> _bottomLevelAccelerationStructures;
        /*
         * first: blas's uuid
         * second:
         *  first: blas ref
         *  second: subMeshGeometryOffset
         */
        ccstd::unordered_map<uint64_t,std::pair<IntrusivePtr<gfx::AccelerationStructure>,uint64_t>> _blasMap;
        /*
         * first: model's uuid
         * second:
         *  first: is alive
         *  second: Acceleration Structure Instance obj
         */
        ccstd::unordered_map<ccstd::string,std::pair<bool,gfx::ASInstance>> _modelMap;
         
        GlobalDSManager* _globalDSManager{nullptr};
    private:

        bool needRebuild = false;
        bool needUpdate = false;
        bool needRecreate = false;
        
        struct subMeshGeomDescriptor{
            uint64_t indexAddress{0};
            uint64_t vertexAddress{0};
        };

        struct meshShadingInstanceDescriptor{
            // The first submesh geometry position of the Mesh
            uint16_t subMeshGeometryOffset{0};
            // The first submesh material position of the Mesh
            uint16_t subMeshMaterialOffset{0};
            uint16_t subMeshCount{0};
            uint16_t padding{0};
        };

        /*
            Definition:
                G = {g1, g2, ...} as all geometries of the model
                M = {m1, m2, ...} as all materials of the model
                T as the transform of the model
                I = {G,M,T} as a unique instance in the scene

            For I1 = {G1,M1,T1} and I2 = {G2,M2,T2}
                iff G1 = G2, then BLAS could be shared.
                iff M1i = M2j, then shader group handle could be shared.
                iff G1 = G2 and M1 = M2, then hit group record could be shared.
        */

        //instanceDecs.geometryOffset + geometryIndex
        ccstd::vector<subMeshGeomDescriptor> _geomDesc;
        ccstd::vector<uint64_t> _materialDesc;
        //instanceCustomIndex
        ccstd::vector<meshShadingInstanceDescriptor> _instanceDesc;

        IntrusivePtr<gfx::Buffer> _geomDescGPUBuffer;
        IntrusivePtr<gfx::Buffer> _materialDescGPUBuffer;
        IntrusivePtr<gfx::Buffer> _instanceDescGPUBuffer;
    };
    

} // namespace pipeline
} // namespace cc
