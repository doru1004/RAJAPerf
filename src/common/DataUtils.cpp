//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
// Copyright (c) 2017-23, Lawrence Livermore National Security, LLC
// and RAJA Performance Suite project contributors.
// See the RAJAPerf/LICENSE file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

#include "DataUtils.hpp"


#include "RAJA/internal/MemUtils_CPU.hpp"

#include <cstdlib>

namespace rajaperf
{

namespace detail
{

static int data_init_count = 0;

/*
 * Reset counter for data initialization.
 */
void resetDataInitCount()
{
  data_init_count = 0;
}

/*
 * Increment counter for data initialization.
 */
void incDataInitCount()
{
  data_init_count++;
}


/*
 * Allocate and initialize aligned data arrays.
 */
void allocAndInitData(Int_ptr& ptr, int len, int align, VariantID vid)
{
  allocData(ptr, len, align, vid);
  initData(ptr, len, vid);
}

void allocAndInitData(Real_ptr& ptr, int len, int align, VariantID vid )
{
  allocData(ptr, len, align, vid);
  initData(ptr, len, vid);
}

void allocAndInitDataConst(Real_ptr& ptr, int len, int align, Real_type val,
                           VariantID vid)
{
  allocData(ptr, len, align, vid);
  initDataConst(ptr, len, val, vid);
}

void allocAndInitDataRandSign(Real_ptr& ptr, int len, int align, VariantID vid)
{
  allocData(ptr, len, align, vid);
  initDataRandSign(ptr, len, vid);
}

void allocAndInitDataRandValue(Real_ptr& ptr, int len, int align, VariantID vid)
{
  allocData(ptr, len, align, vid);
  initDataRandValue(ptr, len, vid);
}

void allocAndInitData(Complex_ptr& ptr, int len, int align, VariantID vid)
{
  allocData(ptr, len, align, vid);
  initData(ptr, len, vid);
}


/*
 * Allocate data arrays of given type.
 */
void allocData(Int_ptr& ptr, int len, int align,
               VariantID vid)
{
  (void)vid;
  ptr = RAJA::allocate_aligned_type<Int_type>(
      align, len*sizeof(Int_type));
}

void allocData(Real_ptr& ptr, int len, int align,
               VariantID vid)
{
  (void)vid;
  ptr = RAJA::allocate_aligned_type<Real_type>(
      align, len*sizeof(Real_type));
}

void allocData(Complex_ptr& ptr, int len, int align,
               VariantID vid)
{
  (void)vid;
  ptr = RAJA::allocate_aligned_type<Complex_type>(
      align, len*sizeof(Complex_type));
}


/*
 * Free data arrays of given type.
 */
void deallocData(Int_ptr& ptr,
                 VariantID vid)
{
  (void)vid;
  if (ptr) {
    RAJA::free_aligned(ptr);
    ptr = nullptr;
  }
}

void deallocData(Real_ptr& ptr,
                 VariantID vid)
{
  (void)vid;
  if (ptr) {
    RAJA::free_aligned(ptr);
    ptr = nullptr;
  }
}

void deallocData(Complex_ptr& ptr,
                 VariantID vid)
{
  (void)vid;
  if (ptr) {
    RAJA::free_aligned(ptr);
    ptr = nullptr;
  }
}


/*
 * \brief Initialize Int_type data array to
 * randomly signed positive and negative values.
 */
void initData(Int_ptr& ptr, int len, VariantID vid)
{
  (void) vid;

// First touch...
#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)
  if ( vid == Base_OpenMP ||
       vid == Lambda_OpenMP ||
       vid == RAJA_OpenMP ) {
    #pragma omp parallel for
    for (int i = 0; i < len; ++i) {
      ptr[i] = 0;
    };
  }
#endif

  srand(4793);

  Real_type signfact = 0.0;

  for (int i = 0; i < len; ++i) {
    signfact = Real_type(rand())/RAND_MAX;
    ptr[i] = ( signfact < 0.5 ? -1 : 1 );
  };

  signfact = Real_type(rand())/RAND_MAX;
  Int_type ilo = len * signfact;
  ptr[ilo] = -58;

  signfact = Real_type(rand())/RAND_MAX;
  Int_type ihi = len * signfact;
  ptr[ihi] = 19;

  incDataInitCount();
}

/*
 * Initialize Real_type data array to non-random
 * positive values (0.0, 1.0) based on their array position
 * (index) and the order in which this method is called.
 */
void initData(Real_ptr& ptr, int len, VariantID vid)
{
  (void) vid;

  Real_type factor = ( data_init_count % 2 ? 0.1 : 0.2 );

// first touch...
#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)
  if ( vid == Base_OpenMP ||
       vid == Lambda_OpenMP ||
       vid == RAJA_OpenMP ) {
    #pragma omp parallel for
    for (int i = 0; i < len; ++i) {
      ptr[i] = factor*(i + 1.1)/(i + 1.12345);
    };
  }
#endif

  for (int i = 0; i < len; ++i) {
    ptr[i] = factor*(i + 1.1)/(i + 1.12345);
  }

  incDataInitCount();
}

/*
 * Initialize Real_type data array to constant values.
 */
void initDataConst(Real_ptr& ptr, int len, Real_type val,
                   VariantID vid)
{

// first touch...
#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)
  if ( vid == Base_OpenMP ||
       vid == Lambda_OpenMP ||
       vid == RAJA_OpenMP ) {
    #pragma omp parallel for
    for (int i = 0; i < len; ++i) {
      ptr[i] = 0;
    };
  }
#else
  (void) vid;
#endif

  for (int i = 0; i < len; ++i) {
    ptr[i] = val;
  };

  incDataInitCount();
}

/*
 * Initialize Real_type data array with random sign.
 */
void initDataRandSign(Real_ptr& ptr, int len, VariantID vid)
{
  (void) vid;

// First touch...
#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)
  if ( vid == Base_OpenMP ||
       vid == Lambda_OpenMP ||
       vid == RAJA_OpenMP ) {
    #pragma omp parallel for
    for (int i = 0; i < len; ++i) {
      ptr[i] = 0.0;
    };
  }
#endif

  Real_type factor = ( data_init_count % 2 ? 0.1 : 0.2 );

  srand(4793);

  for (int i = 0; i < len; ++i) {
    Real_type signfact = Real_type(rand())/RAND_MAX;
    signfact = ( signfact < 0.5 ? -1.0 : 1.0 );
    ptr[i] = signfact*factor*(i + 1.1)/(i + 1.12345);
  };

  incDataInitCount();
}

/*
 * Initialize Real_type data array with random values.
 */
void initDataRandValue(Real_ptr& ptr, int len, VariantID vid)
{
  (void) vid;

// First touch...
#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)
  if ( vid == Base_OpenMP ||
       vid == Lambda_OpenMP ||
       vid == RAJA_OpenMP ) {
    #pragma omp parallel for
    for (int i = 0; i < len; ++i) {
      ptr[i] = 0.0;
    };
  }
#endif

  srand(4793);

  for (int i = 0; i < len; ++i) {
    ptr[i] = Real_type(rand())/RAND_MAX;
  };

  incDataInitCount();
}

/*
 * Initialize Complex_type data array.
 */
void initData(Complex_ptr& ptr, int len, VariantID vid)
{
  (void) vid;

  Complex_type factor = ( data_init_count % 2 ?  Complex_type(0.1,0.2) :
                                                 Complex_type(0.2,0.3) );

#if defined(RAJA_ENABLE_OPENMP) && defined(RUN_OPENMP)
  if ( vid == Base_OpenMP ||
       vid == Lambda_OpenMP ||
       vid == RAJA_OpenMP ) {
    #pragma omp parallel for
    for (int i = 0; i < len; ++i) {
      ptr[i] = factor*(i + 1.1)/(i + 1.12345);
    };
  }
#endif

  for (int i = 0; i < len; ++i) {
    ptr[i] = factor*(i + 1.1)/(i + 1.12345);
  }

  incDataInitCount();
}

/*
 * Initialize scalar data.
 */
void initData(Real_type& d, VariantID vid)
{
  (void) vid;

  Real_type factor = ( data_init_count % 2 ? 0.1 : 0.2 );
  d = factor*1.1/1.12345;

  incDataInitCount();
}

}  // closing brace for detail namespace

/*
 * Calculate and return checksum for data arrays.
 */
long double calcChecksum(const Int_ptr ptr, int len,
                         Real_type scale_factor)
{
  long double tchk = 0.0;
  long double ckahan = 0.0;
  for (Index_type j = 0; j < len; ++j) {
    long double x = (std::abs(std::sin(j+1.0))+0.5) * ptr[j];
    long double y = x - ckahan;
    volatile long double t = tchk + y;
    volatile long double z = t - tchk;
    ckahan = z - y;
    tchk = t;
#if 0 // RDH DEBUG
    if ( (j % 100) == 0 ) {
      getCout() << "j : tchk = " << j << " : " << tchk << std::endl;
    }
#endif
  }
  tchk *= scale_factor;
  return tchk;
}

long double calcChecksum(const Real_ptr ptr, int len,
                         Real_type scale_factor)
{
  long double tchk = 0.0;
  long double ckahan = 0.0;
  for (Index_type j = 0; j < len; ++j) {
    long double x = (std::abs(std::sin(j+1.0))+0.5) * ptr[j];
    long double y = x - ckahan;
    volatile long double t = tchk + y;
    volatile long double z = t - tchk;
    ckahan = z - y;
    tchk = t;
#if 0 // RDH DEBUG
    if ( (j % 100) == 0 ) {
      getCout() << "j : tchk = " << j << " : " << tchk << std::endl;
    }
#endif
  }
  tchk *= scale_factor;
  return tchk;
}

long double calcChecksum(const Complex_ptr ptr, int len,
                         Real_type scale_factor)
{
  long double tchk = 0.0;
  long double ckahan = 0.0;
  for (Index_type j = 0; j < len; ++j) {
    long double x = (std::abs(std::sin(j+1.0))+0.5) * (real(ptr[j])+imag(ptr[j]));
    long double y = x - ckahan;
    volatile long double t = tchk + y;
    volatile long double z = t - tchk;
    ckahan = z - y;
    tchk = t;
#if 0 // RDH DEBUG
    if ( (j % 100) == 0 ) {
      getCout() << "j : tchk = " << j << " : " << tchk << std::endl;
    }
#endif
  }
  tchk *= scale_factor;
  return tchk;
}

}  // closing brace for rajaperf namespace
