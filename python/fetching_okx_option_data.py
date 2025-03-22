import okx.MarketData as MarketData

flag = "0"  # live trading: 0, demo trading: 1

marketDataAPI = MarketData.MarketAPI(flag = flag)

result = marketDataAPI.get_tickers(instType="OPTION",uly="BTC-USD")
# get "BTC-USD-250324-84750-P"
result = marketDataAPI.get_ticker("BTC-USD-250324-84750-P")
result = marketDataAPI.get_orderbook("BTC-USD-250324-84750-P")

# prettey print
import json
print(json.dumps(result, indent=4))