/**
* FFT
* coder: �L���A�K�E�X
* date: 2012/06/19
* message: �����t�[���G�ϊ� (FFT) ���C�u�����ł��B
* http://rexpit.blog29.fc2.com/blog-entry-78.html
**

====
�N���X fft::FftArray
���f���Œ蒷�z��ł��BFFT ���ł��܂��B

-- �g�p��
fft::FftArray z(16);	// �T�C�Y16�̕��f���z��
for (int i = 0; i < z.size(); ++i) {
z[i] = sin(2. * M_PI * i / z.size());
}
z.fft();	// �t�[���G�ϊ�
for (fft::FftArray::iterator it = z.begin(); it != z.end(); ++it) {
std::cout << *it << std::endl;
}
z.ifft();	// �t�[���G�t�ϊ�
for (int i = 0; i < z.size(); ++i) {
std::cout << z[i] << std::endl;
}

*/

#ifndef	FFT_HPP
#define	FFT_HPP

#include <complex>
#include <vector>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>	// std::exit
#ifndef	M_PI
#define	M_PI	3.14159265358979323846
#endif	// M_PI

namespace fft {
	class FftArray {
		typedef std::complex<double> Complex;
		std::vector<Complex> m_x;

		static size_t next2n(size_t x)
		{	// x �ȏ�̍ŏ��� 2^n (n �͎��R��) ��Ԃ�
			size_t y = 1u;
			for (; y < x; y <<= 1u);
			return y;
		}

		void bitReverse()
		{	// �v�f�̈ʒu���A�Y�������r�b�g���]�����ʒu�Ɉړ�
			for (size_t i = 0u; i < m_x.size(); ++i) {
				size_t j = 0u;
				size_t bL = 1u, bR = m_x.size() >> 1u;
				for (; bL < m_x.size(); bL <<= 1u, bR >>= 1u) {	// bit reverse
					if ((i & bL) != 0u) {
						j |= bR;
					}
				}
				if (i < j) {
					std::swap(m_x[i], m_x[j]);
				}
			}
		}

		void doFft(bool isInverse = false) {
			bitReverse();
			for (size_t m = 2u; m <= m_x.size(); m *= 2u) {
				const double arg = (isInverse ? 2. * M_PI : -2. * M_PI) / static_cast<double>(m);
				const Complex w(std::cos(arg), std::sin(arg));
				for (size_t i = 0u; i < m_x.size(); i += m) {
					Complex wj(1., 0.);
					for (size_t j = 0u; j < m / 2u; ++j) {
						const size_t a = i + j;
						const size_t b = i + j + m / 2u;
						const Complex t(wj * m_x[b]);
						m_x[b] = m_x[a] - t;
						m_x[a] = m_x[a] + t;
						wj *= w;
					}
				}
			}
		}

	public:
		typedef std::vector<Complex>::iterator iterator;

		FftArray(size_t n) :
		m_x(next2n(n))
		{
		}

		template <class Iterator>
		FftArray(Iterator first, Iterator last)
			: m_x(next2n(std::distance(first, last)))
		{
			for (iterator it = m_x.begin(); first != last; *it++ = *first++);
		}

		Complex& operator[](int index) {
			if (index < 0) {
				const int add = (-index / static_cast<int>(m_x.size()) + 1) * static_cast<int>(m_x.size());
				index = add + index;
			}
			return m_x[static_cast<size_t>(index) % m_x.size()];
		}

		inline iterator begin() {
			return m_x.begin();
		}

		inline iterator end() {
			return m_x.end();
		}

		inline size_t size() const {
			return m_x.size();
		}

		void resize(size_t s, Complex v = Complex()) {
			m_x.resize(next2n(s), v);
		}

		inline void fft() {
			doFft();
		}

		void ifft() {
			doFft(true);
			for (iterator it = begin(); it != end(); ++it) {
				*it /= static_cast<double>(m_x.size());
			}
		}
	};

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
		std::cout << std::setprecision(15);	// �\������ (���x) ��15���ɕύX
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

}



#endif	// FFT_HPP