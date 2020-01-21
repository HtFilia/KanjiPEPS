import yfinance as yf

msft = yf.Ticker("^GSPC")

hist = msft.history(period="max")

hist.to_csv("../results/S&P500.csv", sep=',')

print("The historical data of S&P500 index has been successfully imported.")
