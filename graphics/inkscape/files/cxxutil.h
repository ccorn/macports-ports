#ifndef CXXUTIL_H
#define CXXUTIL_H

/*
 * Helper functions for C++ < 11
 *
 * Author: Christian Cornelssen
 * Copyright (C) 2017 Christian Cornelssen <ccorn@1tein.de>
 * License: Public domain
 */

#if __cplusplus < 201103L
#include <iterator>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/mpl/logical.hpp>

/*
 * The following operators avoid "no such operator!=" errors
 * due to comparisons of const_reverse_iterator to reverse_iterator
 * in std::vector<T>
 */

template<typename Iterator, typename ConstIterator>
static inline
typename boost::enable_if<boost::mpl::and_<
    boost::is_const<typename boost::remove_pointer<
        BOOST_TYPEOF(&*boost::declval<std::reverse_iterator<ConstIterator> >())
    >::type>,
    boost::mpl::not_<boost::is_const<typename boost::remove_pointer<
        BOOST_TYPEOF(&*boost::declval<std::reverse_iterator<Iterator> >())
    >::type> > >,
bool>::type operator!=(
    const std::reverse_iterator<ConstIterator> &a,
    const std::reverse_iterator<Iterator> &b
) {
    return a != static_cast<std::reverse_iterator<ConstIterator> >(b);
}

template<typename Iterator, typename ConstIterator>
static inline
typename boost::enable_if<boost::mpl::and_<
    boost::is_const<typename boost::remove_pointer<
        BOOST_TYPEOF(&*boost::declval<std::reverse_iterator<ConstIterator> >())
    >::type>,
    boost::mpl::not_<boost::is_const<typename boost::remove_pointer<
        BOOST_TYPEOF(&*boost::declval<std::reverse_iterator<Iterator> >())
    >::type> > >,
bool>::type operator!=(
    const std::reverse_iterator<Iterator> &a,
    const std::reverse_iterator<ConstIterator> &b
) {
    return static_cast<std::reverse_iterator<ConstIterator> >(a) != b;
}
#endif  // __cplusplus < 201103L
#endif	// CXXUTIL_H

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4 :
