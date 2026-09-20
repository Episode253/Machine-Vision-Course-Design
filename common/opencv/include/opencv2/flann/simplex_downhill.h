

#ifndef OPENCV_FLANN_SIMPLEX_DOWNHILL_H_
#define OPENCV_FLANN_SIMPLEX_DOWNHILL_H_

namespace cvflann
{


template <typename T>
void addValue(int pos, float val, float* vals, T* point, T* points, int n)
{
    vals[pos] = val;
    for (int i=0; i<n; ++i) {
        points[pos*n+i] = point[i];
    }


    int j=pos;
    while (j>0 && vals[j]<vals[j-1]) {
        swap(vals[j],vals[j-1]);
        for (int i=0; i<n; ++i) {
            swap(points[j*n+i],points[(j-1)*n+i]);
        }
        --j;
    }
}



template <typename T, typename F>
float optimizeSimplexDownhill(T* points, int n, F func, float* vals = NULL )
{
    const int MAX_ITERATIONS = 10;

    assert(n>0);

    T* p_o = new T[n];
    T* p_r = new T[n];
    T* p_e = new T[n];

    int alpha = 1;

    int iterations = 0;

    bool ownVals = false;
    if (vals == NULL) {
        ownVals = true;
        vals = new float[n+1];
        for (int i=0; i<n+1; ++i) {
            float val = func(points+i*n);
            addValue(i, val, vals, points+i*n, points, n);
        }
    }
    int nn = n*n;

    while (true) {

        if (iterations++ > MAX_ITERATIONS) break;


        for (int j=0; j<n; ++j) {
            p_o[j] = 0;
            for (int i=0; i<n; ++i) {
                p_o[i] += points[j*n+i];
            }
        }
        for (int i=0; i<n; ++i) {
            p_o[i] /= n;
        }

        bool converged = true;
        for (int i=0; i<n; ++i) {
            if (p_o[i] != points[nn+i]) {
                converged = false;
            }
        }
        if (converged) break;


        for (int i=0; i<n; ++i) {
            p_r[i] = p_o[i] + alpha*(p_o[i]-points[nn+i]);
        }
        float val_r = func(p_r);

        if ((val_r>=vals[0])&&(val_r<vals[n])) {


            Logger::info("Choosing reflection\n");
            addValue(n, val_r,vals, p_r, points, n);
            continue;
        }

        if (val_r<vals[0]) {



            for (int i=0; i<n; ++i) {
                p_e[i] = 2*p_r[i]-p_o[i];
            }
            float val_e = func(p_e);

            if (val_e<val_r) {
                Logger::info("Choosing reflection and expansion\n");
                addValue(n, val_e,vals,p_e,points,n);
            }
            else {
                Logger::info("Choosing reflection\n");
                addValue(n, val_r,vals,p_r,points,n);
            }
            continue;
        }
        if (val_r>=vals[n]) {
            for (int i=0; i<n; ++i) {
                p_e[i] = (p_o[i]+points[nn+i])/2;
            }
            float val_e = func(p_e);

            if (val_e<vals[n]) {
                Logger::info("Choosing contraction\n");
                addValue(n,val_e,vals,p_e,points,n);
                continue;
            }
        }
        {
            Logger::info("Full contraction\n");
            for (int j=1; j<=n; ++j) {
                for (int i=0; i<n; ++i) {
                    points[j*n+i] = (points[j*n+i]+points[i])/2;
                }
                float val = func(points+j*n);
                addValue(j,val,vals,points+j*n,points,n);
            }
        }
    }

    float bestVal = vals[0];

    delete[] p_r;
    delete[] p_o;
    delete[] p_e;
    if (ownVals) delete[] vals;

    return bestVal;
}

}

#endif
