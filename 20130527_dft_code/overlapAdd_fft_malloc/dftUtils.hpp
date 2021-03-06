/*!
 * @file dftUtils.cpp
 * @brief DFTÖWÌZÖ
 * @author semitsu
 * @date 2013.07.03
 */
#ifndef	DFTUTILS_HPP
#define	DFTUTILS_HPP
#define FAILURE (-1)
#define SUCCESS (0)

/*!
 * @brief ügÌæÅÌ¡fÏZ@óÔÌæÅÌôÝÝÉ 
 * @param [] 
 */
int fr_multi(double *dest_r, double *dest_i, const double *src_r,const double *src_i, const double *filter_r, const double *filter_i,
	int sampleSize)
;

/*!
 * @brief óÔÌæÅÌôÝÝ
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