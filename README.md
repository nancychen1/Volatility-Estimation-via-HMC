# Volatility Estimation via HMC
 Finds the estimated volatility using Hidden Markov Chains

Utilizing the daily returns in percentage for Exxon Mobil over the 5 year period from 
1/3/2012-12/30/2016 found in the file XOM5YrsDaily.txt, I estimate the historical XOM 
price volatility over this period using a hidden Markov chain approach. We assume the time dependent daily volatility undergoes an exponential random walk. 

After implementing the model and generating XOM's time dependent daily volatility for all 1258 trading days stored in $\sigma_t$, I graphically present the annualized times series ($\sqrt{252}\hat{\sigma_t} : 51 \leq t \leq 1258$) and compare it to the data that the Exponentially Weighted Moving Average (EWMA) produced that is found in EWMA.txt. 
The annualized time series comparison is seen visually in the file EWMA and HMC.png. It is observed that using EWMA and HMC both produce similar outputs. The calculation of the volatility from the EWMA model is estimated based on computing weighted averages and using observed history. Whereas, the HMC model uses the probability of the states to estimate an implied volatility.

I also generate a scatter plot of the points $((x_t, y_t) : 51 ≤ t ≤ 1258)$ where $x_t$ is the EWMA annualized daily volatility in percent and $y_t = \sqrt{252}\hat{σ_t}$ is the HMC annualized daily volatility in percent. This is seen visually in the file EWMA vs HMC.png.  

I also generate a (normal) histogram of the standardized returns $(r_t/\hat{σ_t} : 51 ≤ t ≤ 1258)$ and this is shown in STD Normal Histogram.jpg. It is shown that the standardized returns are distributed normally. When presenting the standardized returns graphically in a time series (STD Return Time Series.jpg), we expect and see 2-3 outliers per year which aligns perfectly with a normal distribution. 

Explanation of Code in HiddenMC.cpp:

First we initialize the values of our free parameters, sigma and alpha, which we say are $0.5$ and $0.038$. We denote these variables as $sig$ and $alpha$, respectively. We want to cycle through time from $0$ to $1258$, denoted by $t$. We then cycle through the states, denoted by $k$, within the previous time loop. However, we have that the first step of a random walk, $W_0$, always equals $0$ thus, we have that $P_0 = 1$. Therefore, we have to cycle through $t$ from $1$ to $1258$.To estimate the volatility, we want to use MAP estimation to find the value of $k$ which satisfies where the conditional probability of the walk at state $k$, at time $t$, is maximized. To find this value, we cycle through all states of $k$ ranging from $[-t, t]$ in increments of $2$ and calculate the conditional probability of the walk at time $t$, given that we are in the state $k$. Thus, we calculate the probability $(*)$ for each $k$:
$$= \frac{p_{k-1} + p_{k+1}}{2}$$

This allows us to find the state $k$, which this probability is maximized. We store it in our variable $khat$. Then we have that the estimated daily volatility at time $t$ is equal to $s_khat$. Thus, we can calculate the volatility: 

    sigma[t] = sig*exp(alpha*khat);


Next, we use Bayesian Updating to update the $p_k$'s which reflect the data through time $t-1$ to reflect the data at time $t$. We loop through all of the $k$ states at each time $t$ and calculate the daily volatility $s_k$ and $g(k)$ which equals: 
$$\frac{1}{\sqrt{2\pi}*s_k} * exp\big( {\frac{-r_t^2}{2s_k^2}} \big) * \mbox{probability}$$

where the probability at each step is calculated from the equation $*$ above.
So our code is as follows:

    s[k] = sig*exp(alpha*k); 
    
    g[k] = 1/(sqrt(2*M_PI)*s[k]) * exp(-R2[t]/(2*s[k]*s[k])) * p_t;

Additionally, we contain the cumulative sum of all the $g_k$'s at time $t$ in the variable, $Z$. Then after exiting the loop for all $k$ states, we loop through once again to update the $p_k$ values. We update using the posterior $\tilde{p_k}$, which is calculated by $\frac{g_k}{Z}$. This is repeated for all $t$ from $1$ to $1258$. 
