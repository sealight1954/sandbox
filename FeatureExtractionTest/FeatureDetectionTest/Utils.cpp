#include "Utils.h"
/*!
 * @brief c:/-----/oooo/mmmm.ext�̃t�@�C���p�X_path�𕪉�
 * @param [dir] c:/-----/oooo
 * @param [fname] mmmm
 * @param [ext] ext
*/
void StdSplitpath(const string &_path, string &_dir, string &_fname, string &_ext)
{
	char drive[256];
	char dir[256];
	char fname[256];
	char ext[256];

	_splitpath(_path.c_str(), drive, dir, fname, ext);
	_dir = dir;
	_fname = fname;
	_ext = ext;
	return;
}

/*!
 * @brief �f�B���N�g�������݂��邩�ǂ����m�F���āA�Ȃ���΃f�B���N�g�����쐬
 * @param []
 * @retval []
*/
void checkMkdir(const string &_path)
{
	struct stat st;
	int ret = stat(_path.c_str(), &st);
	if(ret == 0)
	{
		cerr << "already exist.  :" << _path << endl;

	}
	else
	{
				cerr << "not exist. mkdir:" << _path << endl;
		_mkdir(_path.c_str());
	}

}
