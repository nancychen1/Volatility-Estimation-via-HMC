# Volatility Estimation via HMC
 Finds the estimated volatility using Hidden Markov Chains

Utilizing the daily returns in percentage for Exxon Mobil over the 5 year period from 
1/3/2012-12/30/2016 found in the file XOM5YrsDaily.txt, I estimate the historical XOM 
price volatility over this period using a hidden Markov chain approach. We assume the time dependent daily volatility undergoes an exponential random walk. 

After implementing the model and generating XOM's time dependent daily volatility for all 1258 trading days stored in $\sigma_t$, I graphically present the annualized times series ($\sqrt{252}\
hat{sigma_t} : 51 \leq t \leq 1258$) and compare it to the data that the Exponentially Weighted Moving Average (EWMA) produced that is found in EWMA.txt. 
The annualized time series comparison is seen visually in the file EWMA and HMC.png. It is observed that using EWMA and HMC both produce similar outputs. The calculation of the volatility from the EWMA model is estimated based on computing weighted averages and using observed history. Whereas, the HMC model uses the probability of the states to estimate an implied volatility.

I also generate a scatter plot of the points $((x_t, y_t) : 51 ≤ t ≤ 1258)$ where $x_t$ is the EWMA annualized daily volatility in percent and $y_t = \sqrt{252}\hat{σ_t}$ is the HMC annualized daily volatility in percent. This is seen visually in the file EWMA vs HMC.png.  

I also generate a (normal) histogram of the standardized returns $(r_t/\hat{σ_t} : 51 ≤ t ≤ 1258)$ and this is shown in STD Normal Histogram.jpg. It is shown that the standardized returns are distributed normally. When presenting the standardized returns graphically in a time series (STD Return Time Series.jpg), we expect and see 2-3 outliers per year which aligns perfectly with a normal distribution. 
