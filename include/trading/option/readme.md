# throught
---

the volitility surface, is calculating all iv for options.



For a option market maker, will

- keep fetching the option chain data: Like the tas data for all options.
- store data in ram
- when new trade comes, calculate the iv for the specific option.

So, we need:
- a `Option` class to store the option data
- a `OptionChain` class to store all options data
- a `OptionMarketMaker` class to calculate the iv for the specific option.
