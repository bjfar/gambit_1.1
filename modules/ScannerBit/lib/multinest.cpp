//  GAMBIT: Global and Modular BSM Inference Tool
//  *********************************************
///  \file
///
///  ScannerBit interface to Multinest 3.3
///
///  This interface is based on crapsample.cpp,
///  including random comments from Greg.
///
///  *********************************************
///
///  Authors (add name and date if you modify):
//
///  \author Ben Farmer
///          (ben.farmer@gmail.com)
///  \date 2013 October 2013
///
///  *********************************************

#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <gambit_module.hpp>
#include "priors.hpp"
#include <typeinfo>
#include <cxxabi.h>

// Auxilliary classes and functions needed by multinest
// (cloned largely from eggbox.cc, and modified to use cwrapper.f90 interface instead of multinest.h)

// C++ prototypes for the main run function for multinest, as well as the loglike and dumper functions
extern "C" void run(bool, bool, bool, int, double, double, int, int, int, int, int, double, char[], int, int[], \
                    bool, bool, bool, bool, double, int, double (*)(double*,int,int,void*), \
                    void (*)(int,int,int,double*,double*,double*,double,double,double,void*),void *);
//extern "C" double loglike(double*, int, int, void *);
//extern "C" void dumper(int, int, int, double*, double*, double*,
//                       double, double, double, void*);

namespace Gambit {
   
   namespace MultiNest {

      // Class to connect multinest log-likelihood function and ScannerBit likelihood function
      class LogLikeWrapper
      {
         private:
            // Pointer to a ScannerBit::Function_Base
            ::Gambit::Scanner::Function_Base *boundLogLike;
            // Pointer to the object containing the prior transformation
            const ::Gambit::Priors::BasePrior *boundPrior;
            // Number of free parameters
            int my_ndim;
 
         public:
  
            // Constructor
            // Possibly replace the function pointer to the prior function with something nicer, some virtual base class object or something.
            LogLikeWrapper(::Gambit::Scanner::Function_Base *LogLike, const ::Gambit::Priors::BasePrior &prior, int ndim) 
              : boundLogLike(LogLike), boundPrior(&prior), my_ndim(ndim) 
            {
              // Test that the prior has been bound properly
              //std::vector<double> fakecube(my_ndim,0.5);
              //boundPrior->transform(fakecube);
            }
   
            /******************************************** loglikelihood routine ****************************************************/
            
            // This is the function that will be passed to Multinest as the loglike callback routine
            
            // Input arguments
            // ndim 						= dimensionality (total number of free parameters) of the problem
            // npars 						= total number of free plus derived parameters
            // context						void pointer, any additional information
            //
            // Input/Output arguments
            // Cube[npars] 					= on entry has the ndim parameters in unit-hypercube
            //	 						on exit, the physical parameters plus copy any derived parameters you want to store with the free parameters
            //	 
            // Output arguments
            // lnew 						= loglikelihood
            
            double LogLike(double *Cube, int ndim, int npars)
            {
                   // We need to get the unit interval parameters out of "Cube", transform them to their physical values, and then pass them to the Scanner LogLike function to compute the log likelihood value
                   std::vector<double> unitpars(my_ndim); 
                   std::vector<double> physicalpars(my_ndim);
                   double lnew;
                   int i;
                   // Note: if (ndim!=my_ndim) we are going to have a problem!
   
                   // Extra unit hypercube parameters from Cube
                   for(i = 0; i < ndim; i++)
                   {
                      unitpars[i] = Cube[i];
                          std::cout<<unitpars[i]<<std::endl;
                   }
 
                   if (boundPrior==NULL) {std::cout<<"null pointer!"<<std::endl;}
                   // Transform hypercube parameters to physical values
                   physicalpars = boundPrior->transform(unitpars);
   
                   // Compute log-likelihood
             	   lnew = (*boundLogLike)(physicalpars);
   
                   // Write the physical parameters back into Cube for multinest to write to output file (no other purpose)
                   // (at this point any extra observables that have been computed could also be added to Cube for transfer to the multinest-controlled output files. Must be sufficiently many slots reserved in Cube for this.
                   for(i = 0; i < ndim; i++)
                   {
                      Cube[i] = physicalpars[i];
                   }

                   // Done! (lnew will be used by MultiNest to guide the search)
                   return lnew;                  
            }
   
            /************************************************* dumper routine ******************************************************/
            // (Ben: this is just lifted straight from eggbox.cc with no modification. It doesn't actually do anything really.)         
   
            // The dumper routine will be called every updInt*10 iterations
            // MultiNest doesn not need to the user to do anything. User can use the arguments in whichever way he/she wants
            //
            //
            // Arguments:
            //
            // nSamples 						= total number of samples in posterior distribution
            // nlive 						= total number of live points
            // nPar 						= total number of parameters (free + derived)
            // physLive[1][nlive * (nPar + 1)] 			= 2D array containing the last set of live points (physical parameters plus derived parameters) along with their loglikelihood values
            // posterior[1][nSamples * (nPar + 2)] 			= posterior distribution containing nSamples points. Each sample has nPar parameters (physical + derived) along with the their loglike value & posterior probability
            // paramConstr[1][4*nPar]:
            // paramConstr[0][0] to paramConstr[0][nPar - 1] 	= mean values of the parameters
            // paramConstr[0][nPar] to paramConstr[0][2*nPar - 1] 	= standard deviation of the parameters
            // paramConstr[0][nPar*2] to paramConstr[0][3*nPar - 1] = best-fit (maxlike) parameters
            // paramConstr[0][nPar*4] to paramConstr[0][4*nPar - 1] = MAP (maximum-a-posteriori) parameters
            // maxLogLike						= maximum loglikelihood value
            // logZ							= log evidence value
            // logZerr						= error on log evidence value
            // context						void pointer, any additional information
            
            void dumper(int nSamples, int nlive, int nPar, double *physLive, double *posterior, double *paramConstr, double maxLogLike, double logZ, double logZerr)
            {
            	// convert the 2D Fortran arrays to C++ arrays
            	
            	
            	// the posterior distribution
            	// postdist will have nPar parameters in the first nPar columns & loglike value & the posterior probability in the last two columns
                // Ben: This no longer works since the input types are different from eggbox.cc due to our use of the wrapper in cwrapper.f90. Probably could fix it easily if we really want this stuff for some reason.	
            	//int i, j;
            	
            	//double postdist[nSamples][nPar + 2];
            	//for( i = 0; i < nPar + 2; i++ )
            	//	for( j = 0; j < nSamples; j++ )
            	//		postdist[j][i] = (*posterior)[0][i * nSamples + j];
            		
            	// last set of live points
            	// pLivePts will have nPar parameters in the first nPar columns & loglike value in the last column
            	
            	//double pLivePts[nlive][nPar + 1];
            	//for( i = 0; i < nPar + 1; i++ )
            	//	for( j = 0; j < nlive; j++ )
            	//		pLivePts[j][i] = (*physLive)[0][i * nlive + j];
            }
             
            /***********************************************************************************************************************/   
      };

      // Global pointer to loglikelihood object (multinest seems to be messing up the memory associated with the context pointer when using the new cwrapper.f90 interface)
      LogLikeWrapper *global_loglike_object;

      // Plain-vanilla functions to pass to Multinest for the callback
      
      // So I have always been confused about what the 'context' variable was good for; it seemed pointless. I now have an excellent answer! So here is the story:
      // We need to "dynamically" generate the LogLike function to be run by Multinest. Using a wrapper class whose internal state we can vary does this nicely. However, we cannot obtain regular function pointers to object member functions! The "this" pointer for the object is always required.
      // To get around this, we give Multinest the below "ordinary" functions to work with, and instead pass a pointer to the wrapper object in via the "context" pointer! Then we can do whatever we like with it! Problem solved.
      // Note: we are using the c interface from cwrapper.f90, so the function signature is a little different than in the multinest examples.
      double callback_loglike(double *Cube, int ndim, int npars, void *context)
      {
         // Cast the context pointer into a pointer to a LogLikeWrapper object
         //LogLikeWrapper *loglwrapper;
         //loglwrapper = static_cast<LogLikeWrapper*>(context);

         // Run the likelihood function!
         //return loglwrapper->LogLike(Cube, ndim, npars);

         // using global pointer:
         return global_loglike_object->LogLike(Cube, ndim, npars);
      }

      // Pull the same trick for the dumper function
      void callback_dumper(int nSamples, int nlive, int nPar, double *physLive, double *posterior, double *paramConstr, double maxLogLike, double logZ, double logZerr, void *context)
      {
         // Cast the context pointer into a pointer to a LogLikeWrapper object
         //LogLikeWrapper *loglwrapper;
         //loglwrapper = static_cast<LogLikeWrapper*>(context);

         // Run the dumper function!
         //loglwrapper->dumper(nSamples, nlive, nPar, physLive, posterior, paramConstr, maxLogLike, logZ, logZerr);

         //using global pointer:
         global_loglike_object->dumper(nSamples, nlive, nPar, physLive, posterior, paramConstr, maxLogLike, logZ, logZerr);
      }

   } // End Multinest namespace
} // End Gambit namespace


// Interface to ScannerBit

SCANNER_PLUGIN (multinest)
{
        VERSION(1.0-beta);
        using namespace Gambit::Scanner;

        int PLUGIN_MAIN ()
        {
                std::vector<std::string> &keys     = get_input_value<std::vector<std::string>>(0);
                std::vector<double> &upper_limits  = get_input_value<std::vector<double>>(1);
                std::vector<double> &lower_limits  = get_input_value<std::vector<double>>(2);
                std::vector<std::pair<double,double>> ranges; //Stick limits in here

                //std::string output_file            = get_inifile_value<std::string>("output_file", "default_output");

                // Have to discuss with Greg the best thing to do here.
                Function_Base *LogLike             = GETFUNCTOR("Scanner_Function", "Likelihood");
                //Function_Base *LogLike             = (Function_Base *)(get_input_value<Function_Factory_Base>(3))("Scanner_Function", get_inifile_value<std::string>("like"));
                int ma = keys.size();

                // set the MultiNest sampling parameters 
                // TODO: Transfer these in via ini file
                // NOTE! There is now a flag (called 'outfile') to prevent MultiNest from writing any output files, so once the printer system is working we can safely turn this output off and not be left with all that junk floating around.
        	
        	int IS = 1;					// do Nested Importance Sampling?
        	int mmodal = 0;					// do mode separation?
        	int ceff = 0;					// run in constant efficiency mode?
        	int nlive = 1000;				// number of live points
        	double efr = 0.8;				// set the required efficiency
        	double tol = 0.5;				// tol, defines the stopping criteria
        	int ndims = ma;					// dimensionality (no. of free parameters)
        	int nPar = ndims;					// total no. of parameters including free & derived parameters
        	int nClsPar = ndims;				// no. of parameters to do mode separation on
        	int updInt = 1000;				// after how many iterations feedback is required & the output files should be updated
        							// note: posterior files are updated & dumper routine is called after every updInt*10 iterations
        	double Ztol = -1E90;				// all the modes with logZ < Ztol are ignored
        	int maxModes = 100;				// expected max no. of modes (used only for memory allocation)
        	int pWrap[ndims];				// which parameters to have periodic boundary conditions?
        	for(int i = 0; i < ndims; i++) pWrap[i] = 0;
        	char root[100] = "chains/mnest_test";           // root for output files
        	int seed = -1;					// random no. generator seed, if < 0 then take the seed from system clock
        	int fb = 1;					// need feedback on standard output?
        	int resume = 1;					// resume from a previous job?
        	int outfile = 1;				// write output files?
        	int initMPI = 1;				// initialize MPI routines?, relevant only if compiling with MPI
        							// set it to F if you want your main program to handle MPI initialization
        	double logZero = -1E90;				// points with loglike < logZero will be ignored by MultiNest
        	int maxiter = 0;				// max no. of iterations, a non-positive value means infinity. MultiNest will terminate if either it 
        							// has done max no. of iterations or convergence criterion (defined through tol) has been satisfied
        	void *context = 0;				// not required by MultiNest, any additional information user wants to pass
        
                // Create object to transform from unit hypercube to physical parameters
                ///for (int i = 0; i < ndims; i++)
                ///{
                ///   ranges.push_back(std::make_pair(lower_limits[i],upper_limits[i]));
                ///}
                ///::Gambit::Priors::NdFlatPrior prior(ranges);

                // Demo construction of composite prior
                // use Log prior for first two parameters, and Flat prior for the rest
                typedef std::map<std::vector<int>,::Gambit::Priors::BasePrior*> priorlist_type;
                typedef std::map<std::vector<int>,::Gambit::Priors::BasePrior*>::iterator priorlist_type_it;
                priorlist_type priorlist;
                for (int i = 0; i < ndims; i++)
                {
                   std::vector<int> indices(1,i);
                   if (i<2)
                   {
                      // Add a log prior to the priors list
                      priorlist[indices] = new ::Gambit::Priors::LogPrior(lower_limits[i],upper_limits[i]);
                   }
                   else
                   {
                      // Add a flat prior to the priors list
                      priorlist[indices] = new ::Gambit::Priors::FlatPrior(lower_limits[i],upper_limits[i]);
                   }
                }
                // The downside of this method is that we have to delete the individual prior objects ourselves later on.
                ::Gambit::Priors::CompositePrior prior(priorlist);

                // Create the object which interfaces to the MultiNest LogLike callback function
                // Need to give it the loglikelihood function to evaluate, and the function to perform the prior transformation
                // NOTE TO SELF: Can't pull function pointer out of object like that, since it has a 'this' argument so the call signatures won't match. Just pass in wrapping oject instead.
                ::Gambit::MultiNest::LogLikeWrapper loglwrapper(LogLike, prior, ndims);
       
                // Stick a pointer to the wrapper object into "context" so it can be retrieved by the callback functions
                //context = &loglwrapper;
                
                // using global pointer instead: 	
                ::Gambit::MultiNest::global_loglike_object = &loglwrapper;
 
 
        	// Run MultiNest (supplying callback functions which hook into the interface object, which they know about via the 'context' void pointer)
                std::cout << "Starting multinest..." << std::endl;
       	        run(IS, mmodal, ceff, nlive, tol, efr, ndims, nPar, nClsPar, maxModes, updInt, Ztol, root, seed, pWrap, fb, resume, outfile, initMPI, logZero, maxiter, ::Gambit::MultiNest::callback_loglike, ::Gambit::MultiNest::callback_dumper, context);
                std::cout << "Multinest finished!" << std::endl;
                // Do some cleanup or something.

                // Delete prior objects that we allocated with 'new'
                for (priorlist_type_it it=priorlist.begin(); it!=priorlist.end(); it++)
                {
                  delete (it->second);
                }
     
                return 0;

        }  //end module_main
};
