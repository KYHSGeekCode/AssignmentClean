// AssignmentClean.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://stackoverflow.com/a/51431426/8614565
#include "pch.h"
#include <filesystem>
#include <iostream>
#include <functional>
#include <string>
#include <regex>
namespace fs = std::filesystem;
using namespace std;


#include <iostream>

int main()
{
	wstring str;
	wcout << "Please enter root: \n";
	getline(wcin, str);

	fs::path root(str);		// ch0X
	// const auto root = fs::current_path();
	if (!fs::exists(root)) {
		cout << str << "does not exist" << endl;
		return -1;
	}
	fs::rename(root, root.string() + "_");
	fs::path newRoot = fs::path(root.string() + "_");
	fs::directory_iterator practiceIterator(newRoot);
	fs::create_directory(root);
	for (const auto & dirEntry : practiceIterator) {
		wcmatch match_result;
		const auto& p = dirEntry.path();

		// cout << p.filename() <<endl;
		// cout << p.filename().c_str() << endl;
		if (!regex_match((wchar_t *)(p.filename().c_str()), match_result, wregex(L"practice(\\d{2})"), std::regex_constants::match_default)) {
			wcout << "string literal not matched" << p.filename() << endl;
			continue;
		}

		const  wstring &  number = match_result.str(0);
		wcout << number << endl;
		if (!fs::is_directory(p)) {
			wcout << p << "is not a directory" << endl;
			continue;
		}
		const auto sourceFile = ((p / p.filename()) / (p.filename().wstring() + L".c"));
		const auto execFile = (p / "debug") / (p.filename().wstring() + L".exe");
		wcout << sourceFile << endl;
		wcout << execFile << endl;

		const auto  outDir = (root / wstring(number));
		const auto srcOut = outDir / (L"practice" + wstring(number) + L".c");
		const auto exeOut = outDir / (L"practice" + wstring(number) + L".exe");
		wcout << srcOut << endl;
		wcout << exeOut << endl;
	}
	// Copy only those files which contain "Sub" in their stem.
	/*const auto filter = [](const fs::path& p) -> bool
	{
		return p.stem().generic_string().find("Sub") != std::string::npos;
	};
	CopyRecursive(src, target, filter);*/
	system("pause");
	return 0;
}

// Recursively copies those files and folders from src to target which matches
// predicate, and overwrites existing files in target.
void CopyRecursive(const fs::path& src, const fs::path& target,
	const std::function<bool(fs::path)>& predicate /* or use template */) noexcept
{
	try
	{
		for (const auto& dirEntry : fs::recursive_directory_iterator(src))
		{
			const auto& p = dirEntry.path();
			if (predicate(p))
			{
				// Create path in target, if not existing.
				const auto relativeSrc = fs::relative(p, src);
				const auto targetParentPath = target / relativeSrc.parent_path();
				fs::create_directories(targetParentPath);

				// Copy to the targetParentPath which we just created.
				//fs::copy(p, targetPath, fs::copy_options::overwrite_existing);
			}
		}
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}
}