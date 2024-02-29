#include <include/core/SkImage.h>
#include <Windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <string>
#include <filesystem>

class SystemIcon
{
public:
	SystemIcon();
	~SystemIcon();
	static sk_sp<SkImage> getIcon(SHSTOCKICONID id,const int& size);
	static sk_sp<SkImage> getIcon(GUID& id, const std::wstring& key,const int& size);
	static sk_sp<SkImage> getIcon(std::filesystem::path path, const int& size);
	static void reset();
private:
	static sk_sp<SkImage> iconToImg(HICON icon, const int& size);
};