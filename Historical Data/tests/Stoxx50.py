import yfinance as yf

msft = yf.Ticker("^STOXX50E")

hist = msft.history(period="max")

hist.to_csv("../results/Stoxx50.csv", sep=',')

print("The historical data of Euro Stoxx 50 index has been successfully imported.")
