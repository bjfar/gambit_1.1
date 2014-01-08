//  GAMBIT: Global and Modular BSM Inference Tool
//  *********************************************
///  \file
///
///  Generic observable and likelihood function 
///  macro definitions, for inclusion from actual
///  module source code.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
///   
///  \author Pat Scott
///          (patscott@physics.mcgill.ca)
///  \date 2012 Nov  
///  \date 2013 All year 
///  \date 2014 Foreverrrrr
///
///  \author Abram Krislock
///          (abram.krislock@fysik.su.se)
///  \date 2013 Jan, Feb
//
///  \author Christoph Weniger
///          (c.weniger@uva.nl)
///  \date 2013 Jan, Feb
///
///  \author Anders Kvellestad
///          (anders.kvellestad@fys.uio.no)
///  \date 2013 Nov
///  *********************************************

#ifndef __module_macros_inmodule_hpp__
#define __module_macros_inmodule_hpp__

#include "functors.hpp"
#include "extern_core.hpp"
#include "util_macros.hpp"
#include "safety_bucket.hpp"
#include "module_macros_common.hpp"
#include <boost/preprocessor/control/if.hpp>

/// \name Rollcall macros
/// These are called from within rollcall headers in each module to 
/// register module functions, their capabilities, return types, dependencies,
/// and backend requirements.
/// @{
// Redirect the rollcall macros to their in-module variants
#define START_MODULE                                      DUMMY
#define START_CAPABILITY                                  DUMMY
#define DECLARE_FUNCTION(TYPE, CAN_MANAGE)                MODULE_DECLARE_FUNCTION(TYPE, CAN_MANAGE)
#define DEPENDENCY(DEP, TYPE)                             MODULE_DEPENDENCY(DEP, TYPE)
#define NEEDS_MANAGER_WITH_CAPABILITY(LOOPMAN)            DUMMYARG(LOOPMAN)                                  
#define ALLOWED_MODEL(MODEL)                              MODULE_ALLOWED_MODEL(MODEL)
#define DECLARE_BACKEND_REQ(TYPE, IS_VARIABLE)            MODULE_DECLARE_BACKEND_REQ(TYPE, IS_VARIABLE)
#define BE_OPTION(BACKEND,VERSTRING)                      DUMMYARG(BACKEND,VERSTRING)
#define START_CONDITIONAL_DEPENDENCY(TYPE)                MODULE_START_CONDITIONAL_DEPENDENCY(TYPE)
#define ACTIVATE_DEP_BE(BACKEND_REQ, BACKEND, VERSTRING)  DUMMYARG(BACKEND_REQ, BACKEND, VERSTRING)
#define ACTIVATE_FOR_MODELS(...)                          DUMMYARG(__VA_ARGS__)
/// @}


//  *******************************************************************************
/// \name In-module rollcall macros
/// @{

/// Redirection of \link START_FUNCTION() START_FUNCTION\endlink when invoked 
/// from within a module core.
#define MODULE_DECLARE_FUNCTION(TYPE, CAN_MANAGE)                              \
                                                                               \
  namespace Gambit                                                             \
  {                                                                            \
                                                                               \
    namespace MODULE                                                           \
    {                                                                          \
                                                                               \
      /* Let the module source know that this functor is declared by the core*/\
      namespace Functown { extern module_functor<TYPE> FUNCTION; }             \
                                                                               \
      /* Create a safe pointer to the iteration number of the loop this functor\
      is running within. */                                                    \
      namespace Pipes                                                          \
      {                                                                        \
        namespace FUNCTION                                                     \
        {                                                                      \
          namespace Loop                                                       \
          {                                                                    \
            omp_safe_ptr<int> iteration = Functown::FUNCTION.iterationPtr();   \
          }                                                                    \
        }                                                                      \
      }                                                                        \
                                                                               \
      /* Set up a helper function to call the iterate method if the functor is \
      able to manage loops. */                                                 \
      namespace Functown                                                       \
      {                                                                        \
        BOOST_PP_IIF(CAN_MANAGE,                                               \
          void CAT(FUNCTION,_iterate)(int it)                                  \
          {                                                                    \
            FUNCTION.iterate(it);                                              \
          }                                                                    \
        ,)                                                                     \
      }                                                                        \
                                                                               \
      /* Declare the parameters safe-pointer map as external, and create a     \
      pointer to the single iteration of the loop that can be executed by this \
      functor. */                                                              \
      namespace Pipes                                                          \
      {                                                                        \
        namespace FUNCTION                                                     \
        {                                                                      \
          extern std::map<str, safe_ptr<double> > Param;                       \
          namespace Loop                                                       \
          {                                                                    \
            BOOST_PP_IIF(CAN_MANAGE,                                           \
              void (*executeIteration)(int) =                                  \
               &Functown::CAT(FUNCTION,_iterate);                              \
            ,)                                                                 \
          }                                                                    \
        }                                                                      \
      }                                                                        \
                                                                               \
    }                                                                          \
                                                                               \
  }                                                                            \


/// Redirection of DEPENDENCY(DEP, TYPE) when invoked from within a module.
#define MODULE_DEPENDENCY(DEP, TYPE)                                           \
                                                                               \
  namespace Gambit                                                             \
  {                                                                            \
                                                                               \
    namespace MODULE                                                           \
    {                                                                          \
                                                                               \
      /* Given that TYPE is not void, create a safety_bucket for the           \
      dependency result. To be initialized automatically at runtime            \
      when the dependency is resolved. */                                      \
      namespace Pipes                                                          \
      {                                                                        \
        namespace FUNCTION                                                     \
        {                                                                      \
          BOOST_PP_IIF(IS_TYPE(void,TYPE),,                                    \
            namespace Dep { extern dep_bucket<TYPE> DEP; } )                   \
        }                                                                      \
                                                                               \
      }                                                                        \
                                                                               \
    }                                                                          \
                                                                               \
  }                                                                            \


/// Redirection of ALLOW_MODEL when invoked from within a module.
#define MODULE_ALLOWED_MODEL(MODEL)                                            \
                                                                               \
  namespace Gambit                                                             \
  {                                                                            \
                                                                               \
    namespace MODULE                                                           \
    {                                                                          \
                                                                               \
      /* Create a pointer to the model functor. To be filled by the            \
      dependency resolver during runtime. */                                   \
      namespace Parameters                                                     \
      {                                                                        \
        namespace FUNCTION                                                     \
        {                                                                      \
          extern module_functor<ModelParameters>* MODEL;                       \
        }                                                                      \
      }                                                                        \
                                                                               \
      /* Create a safe pointer to the model parameters result. To be filled    \
      automatically at runtime when the dependency is resolved. */             \
      namespace Pipes                                                   \
      {                                                                        \
        namespace FUNCTION                                                     \
        {                                                                      \
          namespace Model {extern safe_ptr<ModelParameters> MODEL; }           \
        }                                                                      \
      }                                                                        \
                                                                               \
    }                                                                          \
                                                                               \
  }                                                                            \


/// Redirection of START_BACKEND_REQ(TYPE, [VAR/FUNC]) when invoked from within
/// a module. 
#define MODULE_DECLARE_BACKEND_REQ(TYPE, IS_VARIABLE)                          \
                                                                               \
  namespace Gambit                                                             \
  {                                                                            \
    namespace MODULE                                                           \
    {                                                                          \
      namespace Pipes                                                          \
      {                                                                        \
        namespace FUNCTION                                                     \
        {                                                                      \
          namespace BEreq                                                      \
          {                                                                    \
            /* Create a safety_bucket for the backend variable/function.       \
            To be initialized by the dependency resolver at runtime. */        \
            BOOST_PP_IIF(IS_VARIABLE,                                          \
              /* If IS_VARIABLE = 1: */                                        \
              extern BEvariable_bucket<TYPE> BACKEND_REQ;                      \
              , /* If IS_VARAIBLE = 0: */                                      \
              extern BEfunction_bucket<TYPE> BACKEND_REQ;                      \
            ) /* End BOOST_PP_IIF */                                           \
          }                                                                    \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }                                                                            \


/// Redirection of START_CONDITIONAL_DEPENDENCY(TYPE) when invoked from within 
/// a module.
#define MODULE_START_CONDITIONAL_DEPENDENCY(TYPE)                              \
  MODULE_DEPENDENCY(CONDITIONAL_DEPENDENCY, TYPE)                              \

/// @}

#endif // defined __module_macros_inmodule_hpp__ 
