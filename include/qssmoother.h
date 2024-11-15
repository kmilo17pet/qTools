/*!
 * @file qssmoother.h
 * @author J. Camilo Gomez C.
 * @version 1.09
 * @note This file is part of the qLibs distribution.
 * @brief API to smooth noisy signals.
 **/

#ifndef QSIGNAL_SMOOTHER
#define QSIGNAL_SMOOTHER

#ifdef __cplusplus
extern "C" {
#endif

    #include <stdlib.h>
    #include <stdint.h>
    #include "qtdl.h"

    /** @addtogroup qssmoother qSSMoother : Filters to smooth noisy signals
    * @brief API for the qSSmoother library
    *  @{
    */

    /**
    * @brief Supported Filters by the qSSmoother library
    */
    typedef enum {
        QSSMOOTHER_TYPE_LPF1 = 0,   /*!< Low-Pass filter 1st Order*/
        QSSMOOTHER_TYPE_LPF2,       /*!< Low-Pass filter 2nd Order*/
        QSSMOOTHER_TYPE_MWM1,       /*!< Moving Window Median filter ( O(n) time )*/
        QSSMOOTHER_TYPE_MWM2,       /*!< Moving Window Median filter ( O(1) time by using a TDL )*/
        QSSMOOTHER_TYPE_MOR1,       /*!< Moving Outliers Removal ( O(n) time )*/
        QSSMOOTHER_TYPE_MOR2,       /*!< Moving Outliers Removal ( O(1) time by using a TDL )*/
        QSSMOOTHER_TYPE_GMWF,       /*!< Gaussian Filter*/
        QSSMOOTHER_TYPE_KLMN,       /*!< Kalman Filter*/
        QSSMOOTHER_TYPE_EXPW,       /*!< Exponential weighting filter*/
        QSSMOOTHER_TYPE_DESF,       /*!< Double Exponential Smoother*/
        QSSMOOTHER_TYPE_ALNF,       /*!< Adaptive Noise Cancellation*/
    } qSSmoother_Type_t;

    /*! @cond  */
    #define qSSmootherPtr_t  void
    /*abstract class*/
    typedef struct _qSSmoother_s //skipcq: CXX-E2000
    {
        void *vt;
        uint8_t init;
    } _qSSmoother_t; //skipcq: CXX-E2000
    /*! @endcond  */

    /**
    * @brief A 1st order Low-Pass Filter
    */
    typedef struct
    {
        /*! @cond  */
        _qSSmoother_t f;
        float alpha, y1;
        /*! @endcond  */
    } qSSmoother_LPF1_t;

    /**
    * @brief A 2nd order Low-Pass Filter
    */
    typedef struct
    {
        /*! @cond  */
        _qSSmoother_t f;
        float y1, y2, x1, x2;
        float k, a1, a2, b1;
        /*! @endcond  */
    } qSSmoother_LPF2_t;

    /**
    * @brief A Moving Window Median filter
    * @note Time complexity is O(n)
    */
    typedef struct
    {
        /*! @cond  */
        _qSSmoother_t f;
        float *w;
        size_t wsize;
        /*! @endcond  */
    } qSSmoother_MWM1_t;

    /**
    * @brief A Moving Window Median filter
    * @note Time complexity is O(1)
    */
    typedef struct
    {
        /*! @cond  */
        _qSSmoother_t f;
        qTDL_t tdl;
        float sum;
        /*! @endcond  */
    } qSSmoother_MWM2_t;

    /**
    * @brief A Moving Outlier Removal filter
    * @note Time complexity is O(n)
    */
    typedef struct
    {
        /*! @cond  */
        _qSSmoother_t f;
        float *w, m, alpha;
        size_t wsize;
        /*! @endcond  */
    } qSSmoother_MOR1_t;

    /**
    * @brief A Moving Outlier Removal filter
    * @note Time complexity is O(1)
    */
    typedef struct
    {
        /*! @cond  */
        _qSSmoother_t f;
        qTDL_t tdl;
        float sum, m, alpha;
        /*! @endcond  */
    } qSSmoother_MOR2_t;

    /**
    * @brief A Gaussian filter
    */
    typedef struct
    {
        /*! @cond  */
        _qSSmoother_t f;
        float *w, *k;
        size_t wsize;
        /*! @endcond  */
    } qSSmoother_GMWF_t;

    /**
    * @brief An Exponential weighting filter
    */
    typedef struct
    {
        /*! @cond  */
        _qSSmoother_t f;
        float lambda, m, w;
        /*! @endcond  */
    } qSSmoother_EXPW_t;

    /**
    * @brief A scalar Kalman filter
    */
    typedef struct
    {
        /*! @cond  */
        _qSSmoother_t f;
        float x;  /* state */
        float A;  /* x(n)=A*x(n-1)+u(n),u(n)~N(0,q) */
        float H;  /* z(n)=H*x(n)+w(n),w(n)~N(0,r) */
        float q;  /* process(predict) noise covariance */
        float r;  /* measure noise covariance */
        float p;  /* estimated error covariance */
        float gain;
        /*! @endcond  */
    } qSSmoother_KLMN_t;

    /**
    * @brief Double exponential smoothing (Holt’s Method)
    */
    typedef struct
    {
        /*! @cond  */
        _qSSmoother_t f;
        float alpha, beta;
        float lt, bt, n;
        /*! @endcond  */
    } qSSmoother_DESF_t;

    /**
    * @brief Adaptive Filter LMS
    */
    typedef struct
    {
        /*! @cond  */
        _qSSmoother_t f;
        float alpha, mu;
        float *w, *w_1, *x;
        size_t n;
        /*! @endcond  */
    } qSSmoother_ALNF_t;

    /**
    * @brief Check if the smoother filter is initialized.
    * @param[in] s A pointer to the signal smoother instance.
    * @return 1 if the smoother has been initialized, otherwise return 0.
    */
    int qSSmoother_IsInitialized( const qSSmootherPtr_t * const s );

    /**
    * @brief Reset the the smoother filter.
    * @param[in] s A pointer to the signal smoother instance.
    * @return 1 on success, otherwise return 0.
    */
    int qSSmoother_Reset( qSSmootherPtr_t * const s );

    /**
    * @brief Perform the smooth operation recursively for the input signal @a x.
    * @pre Instance must be previously initialized by qSSmoother_Setup()
    * @param[in] s A pointer to the signal smoother instance.
    * @param[in] x A sample of the input signal.
    * @return The smoothed output.
    */
    float qSSmoother_Perform( qSSmootherPtr_t * const s,
                              const float x );

    /**
    * @brief Setup an initialize smoother filter.
    * @param[in] s A pointer to the signal smoother instance.
    * @param[in] type The filter type. Use one of the following values:
    *
    * - ::QSSMOOTHER_TYPE_LPF1
    *
    * - ::QSSMOOTHER_TYPE_LPF2
    *
    * - ::QSSMOOTHER_TYPE_MWM1
    *
    * - ::QSSMOOTHER_TYPE_MWM2
    *
    * - ::QSSMOOTHER_TYPE_MOR1
    *
    * - ::QSSMOOTHER_TYPE_MOR2
    *
    * - ::QSSMOOTHER_TYPE_GMWF
    *
    * - ::QSSMOOTHER_TYPE_KLMN
    *
    * - ::QSSMOOTHER_TYPE_EXPW
    *
    * - ::QSSMOOTHER_TYPE_DESF
    *
    * - ::QSSMOOTHER_TYPE_ALNF
    *
    * @param[in] param The smoother parameters. Depends of the type selected:
    *
    * if ::QSSMOOTHER_TYPE_LPF1 a pointer to a value between  [ 0 < @a alpha < 1 ]
    *
    * if ::QSSMOOTHER_TYPE_LPF2 a pointer to a value between  [ 0 < @a alpha < 1 ]
    *
    * if ::QSSMOOTHER_TYPE_MWM1 can be ignored. Pass @c NULL as argument.
    *
    * if ::QSSMOOTHER_TYPE_MWM2 can be ignored. Pass @c NULL as argument.
    *
    * if ::QSSMOOTHER_TYPE_MOR1 a pointer to a value between  [ 0 < @a alpha < 1 ]
    *
    * if ::QSSMOOTHER_TYPE_MOR2 a pointer to a value between  [ 0 < @a alpha < 1 ]
    *
    * if ::QSSMOOTHER_TYPE_GMWF, an array with two values. The first element
    * with the Standard deviation [ @a sigma > 0 ]. The second element with the
    * offset of the gaussian center. [ 0 < @a offset < ( @a wsize - 1 ) ].
    *
    * if ::QSSMOOTHER_TYPE_KLMN, an array with three values. The first element
    * with the initial estimated error covariance. The second element with the
    * process(predict) noise covariance. The third element with the measure
    * noise convariance
    *
    * if ::QSSMOOTHER_TYPE_EXPW, a pointer to a value between [ 0 < @a lambda < 1 ]
    * that represents the forgetting factor.
    *
    * if ::QSSMOOTHER_TYPE_DESF, an array with three values. The first element
    * [ 0 < @a alpha < 1 ] that represents the weight for the level, the second,
    * [ 0 < @a beta < 1 ] weight for the trend. The third element with the number
    * of step for the forecast, should be an integer value greater or equal to
    * zero.
    *
    * if ::QSSMOOTHER_TYPE_ALNF, an array with two values. The first element
    * with learning rate [ 0 < @a alpha < 1 ]. The second element with the
    * momentum [ 0 < @a mu < 1 ].
    *
    * @param[in] window The filter window and coefficients. Depends of the type
    * selected:
    *
    * if ::QSSMOOTHER_TYPE_LPF1, can be ignored. Pass @c NULL as argument.
    *
    * if ::QSSMOOTHER_TYPE_LPF2, can be ignored. Pass @c NULL as argument.
    *
    * if ::QSSMOOTHER_TYPE_MWM1, An array of @a wsize elements.
    *
    * if ::QSSMOOTHER_TYPE_MWM2, An array of @a wsize elements.
    *
    * if ::QSSMOOTHER_TYPE_MOR1, An array of @a wsize elements.
    *
    * if ::QSSMOOTHER_TYPE_MOR2, An array of @a wsize elements.
    *
    * if ::QSSMOOTHER_TYPE_GMWF, An array of 2* @a wsize to hold both, the
    * window and the gaussian kernel coefficients.
    *
    * if ::QSSMOOTHER_TYPE_KLMN, can be ignored. Pass @c NULL as argument.
    *
    * if ::QSSMOOTHER_TYPE_EXPW, can be ignored. Pass @c NULL as argument.
    *
    * if ::QSSMOOTHER_TYPE_DESF, can be ignored. Pass @c NULL as argument.
    *
    * if ::QSSMOOTHER_TYPE_ALNF, An array of 2* @a wsize elements when momentum
    * is set to zero, otherwise an array of 3* @a wsize elements.
    *
    * @param[in] wsize If used, the number of elements in @a window, otherwise
    * pass 0uL as argument.
    * @return 1 on success, otherwise return 0.
    */
    int qSSmoother_Setup( qSSmootherPtr_t * const s,
                          const qSSmoother_Type_t type,
                          const float * const param,
                          float *window,
                          const size_t wsize );

    /** @}*/

#ifdef __cplusplus
}
#endif

#endif
