import yfinance as yf

msft = yf.Ticker("^HSI")

hist = msft.history(period="max")

hist.to_csv("../results/HangSeng.csv", sep=',')

print("The historical data of Hang Seng index has been successfully imported.")
