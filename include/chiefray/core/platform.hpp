#ifndef CHIEFRAY_PLATFORM_HPP
#define CHIEFRAY_PLATFORM_HPP

#ifdef __CUDACC__
#define CHIEFRAY_HD __host__ __device__
#else
#define CHIEFRAY_HD
#endif

#endif