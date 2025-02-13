/*
*	OXWARE developed by oxiKKK
*	Copyright (c) 2023
* 
*	This program is licensed under the MIT license. By downloading, copying, 
*	installing or using this software you agree to this license.
*
*	License Agreement
*
*	Permission is hereby granted, free of charge, to any person obtaining a 
*	copy of this software and associated documentation files (the "Software"), 
*	to deal in the Software without restriction, including without limitation 
*	the rights to use, copy, modify, merge, publish, distribute, sublicense, 
*	and/or sell copies of the Software, and to permit persons to whom the 
*	Software is furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included 
*	in all copies or substantial portions of the Software. 
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
*	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
*	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
*	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
*	IN THE SOFTWARE.
*/

#include "precompiled.h"

IFileSystem* g_filesystem_i = nullptr;

class CFileSystem : public IFileSystem
{
public:
	CFileSystem();
	~CFileSystem();

	//
	// File system operations
	//

	bool is_directory(const FilePath_t& path);
	bool is_file(const FilePath_t& path);
	bool do_exist(const FilePath_t& path);

	uintmax_t file_size(const FilePath_t& path);

	bool copy(const FilePath_t& from, const FilePath_t& to, ECopyOptions options = ECopyOptions::none);
	bool rename(const FilePath_t& from, const FilePath_t& to);
	bool move(const FilePath_t& from, const FilePath_t& to);
	bool replace(const FilePath_t& what, const FilePath_t& with);
	bool remove(const FilePath_t& path);
	bool remove_all(const FilePath_t& directory);
	bool create_file(const FilePath_t& path); 
	bool create_directory(const FilePath_t& path); 

	void iterate_through_files(const FilePath_t& directory, bool recursive, const std::function<void(const FilePath_t& file)>& callback);

	FilePath_t locate_halflife_dir();
	FilePath_t path_relative_to_hl_dir(const FilePath_t& relative_path = "");
	FilePath_t get_loader_exe_filepath(const FilePath_t& relative_path = "");
	FilePath_t get_appdata_path(const FilePath_t& relative_path = "");
	FilePath_t get_relative_to_appdata(const FilePath_t& full_path);
	FilePath_t get_relative_to_appdata_ex(const FilePath_t& additional_path, const FilePath_t& full_path);

	//
	// Other
	//

	std::error_code get_last_error() const
	{
		return m_last_error;
	}

private:
	std::error_code m_last_error;
};

CFileSystem g_filesystem;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CFileSystem, IFileSystem, IFILESYSTEM_INTERFACEID, g_filesystem);

//-------------------------------------------------------------------------------
//
// CFileSystem implementation
//
//-------------------------------------------------------------------------------

CFileSystem::CFileSystem()
{
	g_filesystem_i = this;

	m_last_error = {};
}

CFileSystem::~CFileSystem()
{
	g_filesystem_i = nullptr;

	m_last_error = {};
}

bool CFileSystem::is_directory(const FilePath_t& path)
{
	return std::filesystem::is_directory(path);
}

bool CFileSystem::is_file(const FilePath_t& path)
{
	return std::filesystem::is_regular_file(path);
}

bool CFileSystem::do_exist(const FilePath_t& path)
{
	return std::filesystem::exists(path);
}

uintmax_t CFileSystem::file_size(const FilePath_t& path)
{
	return std::filesystem::file_size(path);
}

bool CFileSystem::copy(const FilePath_t& from, const FilePath_t& to, ECopyOptions options)
{
	std::filesystem::copy_options cpyo = std::filesystem::copy_options::none;

	if ((options & ECopyOptions::skip_existing) != ECopyOptions::none)
	{
		cpyo |= std::filesystem::copy_options::skip_existing;
	}
	if ((options & ECopyOptions::overwrite_existing) != ECopyOptions::none)
	{
		cpyo |= std::filesystem::copy_options::overwrite_existing;
	}
	if ((options & ECopyOptions::recursive) != ECopyOptions::none)
	{
		cpyo |= std::filesystem::copy_options::recursive;
	}
	if ((options & ECopyOptions::only_directories) != ECopyOptions::none)
	{
		cpyo |= std::filesystem::copy_options::directories_only;
	}

	std::error_code errc;
	std::filesystem::copy(from, to, cpyo, errc);
	if (errc)
	{
		m_last_error = errc;
		return false;
	}

	return true;
}

bool CFileSystem::rename(const FilePath_t& from, const FilePath_t& to)
{
	// filesystem::rename can also move files, we don't want that here
	if (!do_exist(to))
	{
		return false;
	}

	std::error_code errc;
	std::filesystem::rename(from, to, errc);
	if (errc)
	{
		m_last_error = errc;
		return false;
	}

	return true;
}

bool CFileSystem::move(const FilePath_t& from, const FilePath_t& to)
{
	// overwrite the destination with source and remove the source

	if (!copy(from, to, ECopyOptions::overwrite_existing))
	{
		return false;
	}

	if (!remove(from))
	{
		return false;
	}

	return true;
}

bool CFileSystem::replace(const FilePath_t& what, const FilePath_t& with)
{
	// overwrite the destination with source

	if (!copy(what, with, ECopyOptions::overwrite_existing))
	{
		return false;
	}

	return true;
}

bool CFileSystem::remove(const FilePath_t& path)
{
	std::error_code errc;
	bool ret = std::filesystem::remove(path, errc);
	if (errc)
	{
		m_last_error = errc;
		return false;
	}

	return true;
}

bool CFileSystem::remove_all(const FilePath_t& path)
{
	std::error_code errc;
	std::filesystem::remove_all(path, errc);
	if (errc)
	{
		m_last_error = errc;
		return false;
	}

	return true;
}

bool CFileSystem::create_file(const FilePath_t& path)
{
	std::ofstream ofs(path);
	if (!ofs.bad())
	{
		return false;
	}

	return true;
}

bool CFileSystem::create_directory(const FilePath_t& path)
{
	std::error_code errc;
	std::filesystem::create_directory(path, errc);
	if (errc)
	{
		m_last_error = errc;
		return false;
	}

	return true;
}

void CFileSystem::iterate_through_files(const FilePath_t& directory, bool recursive, const std::function<void(const FilePath_t&directory)>& callback)
{
	if (recursive)
	{
		for (const auto& file : std::filesystem::recursive_directory_iterator(directory))
		{
			callback(file.path());
		}
	}
	else
	{
		for (const auto& file : std::filesystem::directory_iterator(directory))
		{
			callback(file.path());
		}
	}
}

FilePath_t CFileSystem::locate_halflife_dir()
{
	const char* path = g_registry_i->read_string(REG_STEAM, "SteamPath");
	if (!path)
	{
		assert(0);
		return {};
	}

	// TODO: Parse 'libraryfolders.vdf' inside 'steamapps/'. There are directories to 
	//		 more steamapps/ directories, if any.

	FilePath_t halflife_dir;
	halflife_dir = FilePath_t(path + std::string("/steamapps/common/Half-Life/"));
	return halflife_dir;
}

FilePath_t CFileSystem::path_relative_to_hl_dir(const FilePath_t& relative_path)
{
	return locate_halflife_dir().string() + relative_path.string();
}

FilePath_t CFileSystem::get_loader_exe_filepath(const FilePath_t& relative_path)
{
	WCHAR path[MAX_PATH];
	g_importbank_i->GetCurrentDirectoryW(sizeof(path), path);
	return FilePath_t(path) / relative_path;
}

FilePath_t CFileSystem::get_appdata_path(const FilePath_t& relative_path)
{
	PWSTR pwstr_appdata_directory;
	HRESULT result = g_importbank_i->SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, NULL, &pwstr_appdata_directory);
	assert(SUCCEEDED(result));
	FilePath_t ret = pwstr_appdata_directory;
	g_importbank_i->CoTaskMemFree(pwstr_appdata_directory);
	return ret / "oxware" / relative_path;
}

FilePath_t CFileSystem::get_relative_to_appdata(const FilePath_t& full_path)
{
	return full_path.lexically_relative(get_appdata_path());
}

FilePath_t CFileSystem::get_relative_to_appdata_ex(const FilePath_t& additional_path, const FilePath_t& full_path)
{
	return full_path.lexically_relative(get_appdata_path(additional_path));
}
