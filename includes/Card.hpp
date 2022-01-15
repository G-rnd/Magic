#ifndef CARD_HPP
#define CARD_HPP

#include <vector>
#include <string>

class Card {

	private:
		int							m_class;
		std::string					m_name;
		int							m_token;

	public:
		Card(int, std::string, int);
		virtual ~Card() = 0;

		virtual int					get_class() const;
		virtual std::string			get_name() const;
		virtual int					get_token() const;

		virtual void				set_name(std::string s);
		virtual void				set_token(int i);

		virtual bool				is_class(int) const;

		virtual std::vector<Card*>	remove(std::vector<Card*> cards);

		virtual void        		print() = 0;

};

#endif
