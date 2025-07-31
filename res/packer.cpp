#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;

struct Entry {
    std::string virtualPath; // "res/shaders/..." etc
    uint32_t offset;         // offset into the data blob
    uint32_t size;           // size of the resource
};

void collectBinFiles(const fs::path& root, const std::string& prefix, std::vector<std::pair<std::string, fs::path>>& outFiles, const std::string& extension) {
  for (auto& p : fs::recursive_directory_iterator(root)) {
    if (p.is_regular_file() && p.path().extension() == extension) {
      std::string relative = fs::relative(p.path(), root).generic_string();
      outFiles.emplace_back(prefix + "/" + relative, p.path());
    }
  }
}

int main() {
  std::vector<std::pair<std::string, fs::path>> files;

  collectBinFiles("generated/shaders", "res/shaders", files, ".bin");
  collectBinFiles("textures", "res/textures", files, ".jpg");
  // collectBinFiles("generated/textures", "res/textures", files);

  std::ofstream out("data.bin", std::ios::binary);
  if (!out) {
    std::cerr << "Failed to open data.bin for writing.\n";
    return 1;
  }

  std::vector<Entry> entries;
  std::vector<std::vector<char>> dataBlobs;

  uint32_t currentOffset = 0;
  for (const auto& [virtPath, realPath] : files) {
    std::ifstream in(realPath, std::ios::binary);
    if (!in) {
      std::cerr << "Failed to read: " << realPath << "\n";
      continue;
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(in)), {});
    Entry e;
    e.virtualPath = virtPath;
    e.offset = currentOffset;
    e.size = static_cast<uint32_t>(buffer.size());

    currentOffset += e.size;
    entries.push_back(e);
    dataBlobs.push_back(std::move(buffer));
  }

  // Write header
  uint32_t fileCount = static_cast<uint32_t>(entries.size());
  out.write(reinterpret_cast<const char*>(&fileCount), sizeof(fileCount));
  for (const auto& e : entries) {
    uint32_t pathLen = static_cast<uint32_t>(e.virtualPath.size());
    out.write(reinterpret_cast<const char*>(&pathLen), sizeof(pathLen));
    out.write(e.virtualPath.data(), pathLen);
    out.write(reinterpret_cast<const char*>(&e.offset), sizeof(e.offset));
    out.write(reinterpret_cast<const char*>(&e.size), sizeof(e.size));
  }

  // Write data section
  for (const auto& blob : dataBlobs) {
    out.write(blob.data(), blob.size());
  }

  out.close();
  std::cout << "Packed " << fileCount << " resources into data.bin\n";
  return 0;
}
