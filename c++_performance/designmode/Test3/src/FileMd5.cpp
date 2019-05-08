#include "FileMd5.h"
FileMd5 FileMd5::fileMd5;
FileMd5::FileMd5()
{
}
bool FileMd5::GetMd5(const char* filepath, string& md5str)
{
	if (false == ExceptionCheck(filepath))
	{
		cerr << "file path is null or file can not be open...!" << endl;
		return false;
	}
	MD5_CTX md5_context;
	GetMd5ContextFromFile(md5_context);
	ComputeMd5Value(md5_context, md5str);
	read_file_stream.close();
	return true;
}
bool FileMd5::ExceptionCheck(const char *filepath)
{
	if (nullptr == filepath)
	{
		return false;
	}
	read_file_stream.open(filepath, ios::in | ios::binary);
	return read_file_stream.is_open();
}
void FileMd5::GetMd5ContextFromFile(MD5_CTX &md5_context)
{
	MD5_Init(&md5_context);
	char buf[BUFF_SIZE] = "";
	while (read_file_stream.good())
	{
		read_file_stream.read(buf, sizeof(buf));
		MD5_Update(&md5_context, buf, read_file_stream.gcount());
	}
}
void FileMd5::ComputeMd5Value(MD5_CTX &md5_context, string &md5str)
{
	unsigned char result[MD5_DIGEST_LENGTH] = "";
	MD5_Final(result, &md5_context);
	char hex[35] = "";
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
	{
		snprintf(hex + (i << 1), 3, "%02x", result[i]);
	}
	md5str.assign(hex);
}
void FileMd5::UpdateMd5Cache(const char* filepath, const char* md5str)
{
	lock_guard lk(global_lock);
	if (end(map_path_md5) != map_path_md5.find(filepath))
	{
		return;
	}
	map_path_md5[filepath] = md5str;
}
bool FileMd5::FileExist(const char* filepath)
{
	string md5str;
	if (false == GetMd5(filepath, md5str))
	{
		return true;
	}
	lock_guard lk(global_lock);
	return end(map_path_md5) != map_path_md5.find(filepath);
}
FileMd5::~FileMd5()
{
}
