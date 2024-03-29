#!/usr/bin/env python
# -*- coding: utf-8 -*-


from __future__ import print_function

from . import Ticker, multi
from collections import namedtuple as _namedtuple


def genTickers(tickers):
    tickers = tickers if isinstance(
        tickers, list) else tickers.replace(',', ' ').split()
    tickers = [ticker.upper() for ticker in tickers]
    ticker_objects = {}

    for ticker in tickers:
        ticker_objects[ticker] = Ticker(ticker)
    return _namedtuple("Tickers", ticker_objects.keys()
                       )(*ticker_objects.values())


class Tickers():

    def __repr__(self):
        return 'yfinance.Tickers object <%s>' % ",".join(self.symbols)

    def __init__(self, tickers):
        tickers = tickers if isinstance(
            tickers, list) else tickers.replace(',', ' ').split()
        self.symbols = [ticker.upper() for ticker in tickers]
        ticker_objects = {}

        for ticker in self.symbols:
            ticker_objects[ticker] = Ticker(ticker)

        self.tickers = _namedtuple(
            "Tickers", ticker_objects.keys(), rename=True
        )(*ticker_objects.values())

    def history(self, period="1mo", interval="1d",
                start=None, end=None, prepost=False,
                actions=True, auto_adjust=True, proxy=None,
                threads=True, group_by='column', progress=True,
                **kwargs):

        return self.download(
                period, interval,
                start, end, prepost,
                actions, auto_adjust, proxy,
                threads, group_by, progress,
                **kwargs)

    def download(self, period="1mo", interval="1d",
                 start=None, end=None, prepost=False,
                 actions=True, auto_adjust=True, proxy=None,
                 threads=True, group_by='column', progress=True,
                 **kwargs):

        data = multi.download(self.symbols,
                              start=start, end=end,
                              actions=actions,
                              auto_adjust=auto_adjust,
                              period=period,
                              interval=interval,
                              prepost=prepost,
                              proxy=proxy,
                              group_by='ticker',
                              threads=threads,
                              progress=progress,
                              **kwargs)

        for symbol in self.symbols:
            getattr(self.tickers, symbol)._history = data[symbol]

        if group_by == 'column':
            data.columns = data.columns.swaplevel(0, 1)
            data.sort_index(level=0, axis=1, inplace=True)

        return data
