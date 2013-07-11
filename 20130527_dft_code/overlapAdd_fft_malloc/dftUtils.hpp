/*!
 * @file dftUtils.cpp
 * @brief DFT�֌W�̉��Z�֐�
 * @author semitsu
 * @date 2013.07.03
 */
#ifndef	DFTUTILS_HPP
#define	DFTUTILS_HPP
#define FAILURE (-1)
#define SUCCESS (0)

/*!
 * @brief ���g���̈�ł̕��f�ώZ�@��ԗ̈�ł̏�ݍ��݂ɑ��� 
 * @param [] 
 */
int fr_multi(double *dest_r, double *dest_i, const double *src_r,const double *src_i, const double *filter_r, const double *filter_i,
	int sampleSize)
;

/*!
 * @brief ��ԗ̈�ł̏�ݍ���
 * @param [] 
 */
int conv(double *dest_r, const double *src_r, const double *filter_r,
	int sampleSize);
/*!
 * @brief DFT
 * @param [] 
 */
int dft(double *dest_r, double *dest_i,
	const double *src_r, const double *src_i, 
	int sampleSize, bool inverse);



#endif