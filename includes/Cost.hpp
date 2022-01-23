#ifndef COST_HPP
#define COST_HPP

#include <string>

class Cost {

	private:
		int			m_any;
		int			m_white;
		int			m_blue;
		int			m_black;
		int			m_red;
		int			m_green;

	public:
		Cost(int, int, int, int, int, int);
		Cost(const Cost &other);

		~Cost();

		int			get_any() const;
		int			get_white() const;
		int			get_black() const;
		int			get_red() const;
		int			get_green() const;
		int			get_blue() const;

		void		set_any(int);
		void		set_white(int);
		void		set_black(int);
		void		set_red(int);
		void		set_green(int);
		void		set_blue(int);

		void		print() const;
		bool		is_null();
		bool		is_color_null();
		bool		is_any_null();
		bool		is_white_null();
		bool		is_black_null();
		bool		is_red_null();
		bool		is_green_null();
		bool		is_blue_null();

		std::string to_string();
		Cost&		operator=(const Cost& o);
	
};

#endif
