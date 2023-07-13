#include <unordered_map>
#include <string>

class GPUMesh;
class Mesh;
class Model;

class GPUManager
{
private:
	std::unordered_map<std::string, GPUMesh*> meshes;
public:
	GPUMesh* GetMesh(std::string const &str);
	GPUMesh* AddMesh(std::string const &name, GPUMesh *mesh);

	GPUMesh* CreateMesh(std::string const &name, Mesh *mesh);
	GPUMesh* CreateMesh(std::string const &name, Model *model);
};