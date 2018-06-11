//
// Copyright (c) 2017 Christopher Baker <https://christopherbaker.net>
//
// SPDX-License-Identifier:	MIT
//


#include "ofApp.h"


void ofApp::setup()
{
    // Declare some variables used below
    dlib::matrix<double,3,1> y;
    dlib::matrix<double,3,3> M;
    dlib::matrix<double> x;

    // set all elements to 1
    y = 1;
    M = 1;


    // -------------------------  Template Expressions -----------------------------
    // Now I will discuss the "template expressions" technique and how it is
    // used in the matrix object.  First consider the following expression:
    x = y + y;

    /*
     Normally this expression results in machine code that looks, at a high
     level, like the following:
     temp = y + y;
     x = temp

     Temp is a temporary matrix returned by the overloaded + operator.
     temp then contains the result of adding y to itself.  The assignment
     operator copies the value of temp into x and temp is then destroyed while
     the blissful C++ user never sees any of this.

     This is, however, totally inefficient.  In the process described above
     you have to pay for the cost of constructing a temporary matrix object
     and allocating its memory.  Then you pay the additional cost of copying
     it over to x.  It also gets worse when you have more complex expressions
     such as x = round(y + y + y + M*y) which would involve the creation and copying
     of 5 temporary matrices.

     All these inefficiencies are removed by using the template expressions
     technique.  The basic idea is as follows, instead of having operators and
     functions return temporary matrix objects you return a special object that
     represents the expression you wish to perform.

     So consider the expression x = y + y again.  With dlib::matrix what happens
     is the expression y+y returns a matrix_exp object instead of a temporary matrix.
     The construction of a matrix_exp does not allocate any memory or perform any
     computations.  The matrix_exp however has an interface that looks just like a
     dlib::matrix object and when you ask it for the value of one of its elements
     it computes that value on the spot.  Only in the assignment operator does
     someone ask the matrix_exp for these values so this avoids the use of any
     temporary matrices.  Thus the statement x = y + y is equivalent to the following
     code:
     // loop over all elements in y matrix
     for (long r = 0; r < y.nr(); ++r)
     for (long c = 0; c < y.nc(); ++c)
     x(r,c) = y(r,c) + y(r,c);


     This technique works for expressions of arbitrary complexity.  So if you typed
     x = round(y + y + y + M*y) it would involve no temporary matrices being created
     at all.  Each operator takes and returns only matrix_exp objects.  Thus, no
     computations are performed until the assignment operator requests the values
     from the matrix_exp it receives as input.  This also means that statements such as:
     auto x = round(y + y + y + M*y)
     will not work properly because x would be a matrix expression that references
     parts of the expression round(y + y + y + M*y) but those expression parts will
     immediately go out of scope so x will contain references to non-existing sub
     matrix expressions.  This is very bad, so you should never use auto to store
     the result of a matrix expression.  Always store the output in a matrix object
     like so:
     matrix<double> x = round(y + y + y + M*y)




     In terms of implementation, there is a slight complication in all of this.  It
     is for statements that involve the multiplication of a complex matrix_exp such
     as the following:
     */
    x = M*(M+M+M+M+M+M+M);
    /*
     According to the discussion above, this statement would compute the value of
     M*(M+M+M+M+M+M+M) totally without any temporary matrix objects.  This sounds
     good but we should take a closer look.  Consider that the + operator is
     invoked 6 times.  This means we have something like this:

     x = M * (matrix_exp representing M+M+M+M+M+M+M);

     M is being multiplied with a quite complex matrix_exp.  Now recall that when
     you ask a matrix_exp what the value of any of its elements are it computes
     their values *right then*.

     If you think on what is involved in performing a matrix multiply you will
     realize that each element of a matrix is accessed M.nr() times.  In the
     case of our above expression the cost of accessing an element of the
     matrix_exp on the right hand side is the cost of doing 6 addition operations.

     Thus, it would be faster to assign M+M+M+M+M+M+M to a temporary matrix and then
     multiply that by M.  This is exactly what the dlib::matrix does under the covers.
     This is because it is able to spot expressions where the introduction of a
     temporary is needed to speed up the computation and it will automatically do this
     for you.




     Another complication that is dealt with automatically is aliasing.  All matrix
     expressions are said to "alias" their contents.  For example, consider
     the following expressions:
     M + M
     M * M

     We say that the expressions (M + M) and (M * M) alias M.  Additionally, we say that
     the expression (M * M) destructively aliases M.

     To understand why we say (M * M) destructively aliases M consider what would happen
     if we attempted to evaluate it without first assigning (M * M) to a temporary matrix.
     That is, if we attempted to perform the following:
     for (long r = 0; r < M.nr(); ++r)
     for (long c = 0; c < M.nc(); ++c)
     M(r,c) = rowm(M,r)*colm(M,c);

     It is clear that the result would be corrupted and M wouldn't end up with the right
     values in it.  So in this case we must perform the following:
     temp = M*M;
     M = temp;

     This sort of interaction is what defines destructive aliasing.  Whenever we are
     assigning a matrix expression to a destination that is destructively aliased by
     the expression we need to introduce a temporary.  The dlib::matrix is capable of
     recognizing the two forms of aliasing and introduces temporary matrices only when
     necessary.
     */



    // Next we discuss how to create custom matrix expressions.   In what follows we
    // will define three different matrix expressions and show their use.
    custom_matrix_expressions_example();

    // The end.
    ofExit();
}


void ofApp::custom_matrix_expressions_example()
{
    dlib::matrix<double> x(2,3);
    x = 1, 1, 1,
    2, 2, 2;

    cout << x << endl;

    // Finally, let's use the matrix expressions we defined above.

    // prints the transpose of x
    cout << example_trans(x) << endl;

    // prints this:
    //   11 11 11
    //   12 12 12
    cout << add_scalar(x, 10) << endl;


    // matrix expressions can be nested, even the user defined ones.
    // the following statement prints this:
    //   11 12
    //   11 12
    //   11 12
    cout << example_trans(add_scalar(x, 10)) << endl;

    // Since we setup the alias detection correctly we can even do this:
    x = example_trans(add_scalar(x, 10));
    cout << "new x:\n" << x << endl;

    cout << "Do some testing with the example_vector_to_matrix() function: " << endl;
    std::vector<float> vect;
    vect.push_back(1);
    vect.push_back(3);
    vect.push_back(5);

    // Now let's treat our std::vector like a matrix and print some things.
    cout << example_vector_to_matrix(vect) << endl;
    cout << add_scalar(example_vector_to_matrix(vect), 10) << endl;

    /*
     As an aside, note that dlib contains functions equivalent to the ones we
     defined above.  They are:
     - dlib::trans()
     - dlib::mat() (converts things into matrices)
     - operator+ (e.g. you can say my_mat + 1)


     Also, if you are going to be creating your own matrix expression you should also
     look through the matrix code in the dlib/matrix folder.  There you will find
     many other examples of matrix expressions.
     */
}


