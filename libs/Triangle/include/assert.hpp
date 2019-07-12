
namespace tpp {

/*! \file assert.hpp
        \brief Implements a better 'Assert'
 */

#ifndef REVIVER_ASSERT_HPP
#define REVIVER_ASSERT_HPP


/*! \def MyAssertFunction
    \brief Function used by 'Assert' function in _DEBUG mode.
   
    Details.
*/
extern bool MyAssertFunction( bool b, const char* desc, int line, const char* file);


#if defined( _DEBUG ) && defined(YOU_KNOW_WHAT_DONT_EVER_DO_THAT)
#define Assert( exp, description ) MyAssertFunction( (int)(exp), description, __LINE__, __FILE__ )
#else
#define Assert( exp, description )
#endif


#endif

}
