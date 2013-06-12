#include "fft.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>	// std::exit

static const char *USAGE_TEXT = "\
								Usage\n\
								\n\
								Options\n\
								-i	Inverse FFT\n\
								-c	Complex input\n\
								-a	Absolute output\n\
								-r	Output only real number\n\
								-h	This page";

class Fft {
	bool flagInverse;
	bool flagComplex;
	bool flagAbsolute;
	bool flagReal;

	fft::FftArray input() const
	{
		typedef std::complex<double> Complex;

		std::vector<Complex> v;
		if (flagComplex) {
			double re, im;
			while (std::cin >> re >> im) {
				v.push_back(Complex(re, im));
			}
		} else {
			double re;
			while (std::cin >> re) {
				v.push_back(Complex(re, 0.));
			}
		}

		return fft::FftArray(v.begin(), v.end());
	}

	template <class Iterator>
	void output(Iterator first, Iterator last) const
	{
		std::cout << std::setprecision(15);	// •\Ž¦Œ…” (¸“x) ‚ð15Œ…‚É•ÏX
		for (Iterator it = first; it != last; ++it) {
			if (flagAbsolute) {
				std::cout << abs(*it) << std::endl;
			} else if (flagReal) {
				std::cout << it->real() << std::endl;
			} else {
				std::cout << it->real() << '\t' << it->imag() << std::endl;
			}
		}
	}

public:
	Fft(const int argc, const char *argv[])
		: flagInverse(false), flagComplex(false), flagAbsolute(false), flagReal(false)
	{
		const std::vector<std::string> args(argv, argv + argc);
		std::vector<std::string>::const_iterator it = args.begin();
		for (++it; it != args.end(); ++it) {
			const std::string str = *it;
			if (str.find("i") != std::string::npos) {
				flagInverse = true;
			}
			if (str.find("c") != std::string::npos) {
				flagComplex = true;
			}
			if (str.find("a") != std::string::npos) {
				flagAbsolute = true;
			}
			if (str.find("r") != std::string::npos) {
				flagReal = true;
			}
			if (str.find("h") != std::string::npos) {
				std::cout << USAGE_TEXT << std::endl;
				std::exit(0);
			}
		}
	}

	void interpret() const
	{
		fft::FftArray z(input());

		// FFT or IFFT
		if (flagInverse) {
			z.ifft();
		} else {
			z.fft();
		}

		output(z.begin(), z.end());
	}
};

int main(const int argc, const char *argv[])
{

	int datasize = 4;
	double ti_src_r[] = {1,2,3,4};
	double ti_src_i[] = {0,0,0,0};
	double fr_src_r[] = {0,0,0,0};
	double fr_src_i[] = {0,0,0,0};
	double fr_multi_src_r[] = {0,0,0,0};
	double fr_multi_src_i[] = {0,0,0,0};
	double ti_multi_dst_r[] = {0,0,0,0};
	double ti_multi_dst_i[] = {0,0,0,0};
	double ti_conv_dst_r[] = {0,0,0,0};
	double ti_conv_dst_i[] = {0,0,0,0};

	double filter_r[]= {0,1,0,0};
	double filter_i[]={0,0,0,0};
	double fr_filter_r[]={0,0,0,0};
	double fr_filter_i[]={0,0,0,0};
	Fft fft(argc,argv);
	fft::FftArray arr(4);
	arr[0]=1;
	arr[1]=2;
	arr[2]=3;
	arr[3]=4;
	arr.fft();
	arr.ifft();
	return 0;
}