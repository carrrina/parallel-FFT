# parallel-FFT
Parallel, scalable with the number of threads, version of the Fourier Transform and Fast Fourier Transform algorithms, in C using the Pthread API.

Fourier Transform

  A Fourier Transform is an extremely useful mathematical tool. It decomposes a function into the frequencies
that make it up. In other words, it converts a signal from its original domain, time to a representation in
the frequency domain.
  The Fourier Transform has multiple uses, especially in digital signal processing, where it can be used for
problems as complex as separating voice from music. Other interesting uses span from image compression
to applications in financial analysis.
  Computing the Discreet Transform will have a complexity of O(N^2).

Fast Fourier Transform  

  The Fast Fourier Transform uses the symmetry of the function required to calculate the Fourier Transform
to reuse much of the underlying computation and leads to time complexity of O(N*logN).
  The code only works if the number of elements is a power of 2.
