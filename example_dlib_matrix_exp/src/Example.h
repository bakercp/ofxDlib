#pragma once


template <typename M>
struct example_op_trans
{
    /*!
     This object defines a matrix expression that represents a transposed matrix.
     As discussed above, constructing this object doesn't compute anything.  It just
     holds a reference to a matrix and presents an interface which defines
     matrix transposition.
     !*/

    // Here we simply hold a reference to the matrix we are supposed to be the transpose of.
    example_op_trans( const M& m_) : m(m_){}
    const M& m;

    // The cost field is used by matrix multiplication code to decide if a temporary needs to
    // be introduced.  It represents the computational cost of evaluating an element of the
    // matrix expression.  In this case we say that the cost of obtaining an element of the
    // transposed matrix is the same as obtaining an element of the original matrix (since
    // transpose doesn't really compute anything).
    const static long cost = M::cost;

    // Here we define the matrix expression's compile-time known dimensions.  Since this
    // is a transpose we define them to be the reverse of M's dimensions.
    const static long NR = M::NC;
    const static long NC = M::NR;

    // Define the type of element in this matrix expression.  Also define the
    // memory manager type used and the layout.  Usually we use the same types as the
    // input matrix.
    typedef typename M::type type;
    typedef typename M::mem_manager_type mem_manager_type;
    typedef typename M::layout_type layout_type;

    // This is where the action is.  This function is what defines the value of an element of
    // this matrix expression.  Here we are saying that m(c,r) == trans(m)(r,c) which is just
    // the definition of transposition.  Note also that we must define the return type from this
    // function as a typedef.  This typedef lets us either return our argument by value or by
    // reference.  In this case we use the same type as the underlying m matrix.  Later in this
    // example program you will see two other options.
    typedef typename M::const_ret_type const_ret_type;
    const_ret_type apply (long r, long c) const { return m(c,r); }

    // Define the run-time defined dimensions of this matrix.
    long nr () const { return m.nc(); }
    long nc () const { return m.nr(); }

    // Recall the discussion of aliasing.  Each matrix expression needs to define what
    // kind of aliasing it introduces so that we know when to introduce temporaries.  The
    // aliases() function indicates that the matrix transpose expression aliases item if
    // and only if the m matrix aliases item.
    template <typename U> bool aliases               ( const dlib::matrix_exp<U>& item) const { return m.aliases(item); }
    // This next function indicates that the matrix transpose expression also destructively
    // aliases anything m aliases.  I.e. transpose has destructive aliasing.
    template <typename U> bool destructively_aliases ( const dlib::matrix_exp<U>& item) const { return m.aliases(item); }

};


// Here we define a simple function that creates and returns transpose expressions.  Note that the
// matrix_op<> template is a matrix_exp object and exists solely to reduce the amount of boilerplate
// you have to write to create a matrix expression.
template < typename M >
const dlib::matrix_op<example_op_trans<M> > example_trans (
                                                     const dlib::matrix_exp<M>& m
                                                     )
{
    typedef example_op_trans<M> op;
    // m.ref() returns a reference to the object of type M contained in the matrix expression m.
    return dlib::matrix_op<op>(op(m.ref()));
}

// ----------------------------------------------------------------------------------------

template <typename T>
struct example_op_vector_to_matrix
{
    /*!
     This object defines a matrix expression that holds a reference to a std::vector<T>
     and makes it look like a column vector.  Thus it enables you to use a std::vector
     as if it was a dlib::matrix.

     !*/
    example_op_vector_to_matrix( const std::vector<T>& vect_) : vect(vect_){}

    const std::vector<T>& vect;

    // This expression wraps direct memory accesses so we use the lowest possible cost.
    const static long cost = 1;

    const static long NR = 0; // We don't know the length of the vector until runtime.  So we put 0 here.
    const static long NC = 1; // We do know that it only has one column (since it's a vector)
    typedef T type;
    // Since the std::vector doesn't use a dlib memory manager we list the default one here.
    typedef dlib::default_memory_manager mem_manager_type;
    // The layout type also doesn't really matter in this case.  So we list row_major_layout
    // since it is a good default.
    typedef dlib::row_major_layout layout_type;

    // Note that we define const_ret_type to be a reference type.  This way we can
    // return the contents of the std::vector by reference.
    typedef const T& const_ret_type;
    const_ret_type apply (long r, long ) const { return vect[r]; }

    long nr () const { return vect.size(); }
    long nc () const { return 1; }

    // This expression never aliases anything since it doesn't contain any matrix expression (it
    // contains only a std::vector which doesn't count since you can't assign a matrix expression
    // to a std::vector object).
    template <typename U> bool aliases               ( const dlib::matrix_exp<U>& ) const { return false; }
    template <typename U> bool destructively_aliases ( const dlib::matrix_exp<U>& ) const { return false; }
};

template < typename T >
const dlib::matrix_op<example_op_vector_to_matrix<T> > example_vector_to_matrix (
                                                                                 const std::vector<T>& vector
                                                                                 )
{
    typedef example_op_vector_to_matrix<T> op;
    return dlib::matrix_op<op>(op(vector));
}

// ----------------------------------------------------------------------------------------

template <typename M, typename T>
struct example_op_add_scalar
{
    /*!
     This object defines a matrix expression that represents a matrix with a single
     scalar value added to all its elements.
     !*/

    example_op_add_scalar( const M& m_, const T& val_) : m(m_), val(val_){}

    // A reference to the matrix
    const M& m;
    // A copy of the scalar value that should be added to each element of m
    const T val;

    // This time we add 1 to the cost since evaluating an element of this
    // expression means performing 1 addition operation.
    const static long cost = M::cost + 1;
    const static long NR = M::NR;
    const static long NC = M::NC;
    typedef typename M::type type;
    typedef typename M::mem_manager_type mem_manager_type;
    typedef typename M::layout_type layout_type;

    // Note that we declare const_ret_type to be a non-reference type.  This is important
    // since apply() computes a new temporary value and thus we can't return a reference
    // to it.
    typedef type const_ret_type;
    const_ret_type apply (long r, long c) const { return m(r,c) + val; }

    long nr () const { return m.nr(); }
    long nc () const { return m.nc(); }

    // This expression aliases anything m aliases.
    template <typename U> bool aliases               ( const dlib::matrix_exp<U>& item) const { return m.aliases(item); }
    // Unlike the transpose expression.  This expression only destructively aliases something if m does.
    // So this expression has the regular non-destructive kind of aliasing.
    template <typename U> bool destructively_aliases ( const dlib::matrix_exp<U>& item) const { return m.destructively_aliases(item); }
    
}; 


template < typename M, typename T >
const dlib::matrix_op<example_op_add_scalar<M,T>> add_scalar (
                                                        const dlib::matrix_exp<M>& m,
                                                         T val
                                                         )
{
    typedef example_op_add_scalar<M,T> op;
    return dlib::matrix_op<op>(op(m.ref(), val));
}
