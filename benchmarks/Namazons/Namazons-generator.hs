#!/usr/bin/stack
{- stack --resolver lts-6.5 --install-ghc runghc -}

module Main where

import System.Environment

main :: IO ()
main = do
  args <- getArgs
  case args of
    [nstr] -> do
      putStrLn $ "c " ++ show n ++ " amazons. see https://de.wikipedia.org/wiki/Damenproblem#Andere_Figuren"
      putStr . unlines $ domainLines ++ clauses where
        n,m :: Int
        n = read nstr
        m = n - 1
        -- pLine = "p mvcnf " ++ show n ++ " " ++ show (length clauses - 3)
        domainLines = "c one variable per row, value is the column to place an amazon:" :
          [ "d " ++ show i ++ " " ++ show n | i <- [1..n] ]
        clauses = alldiff ++ diagonal ++ knightmoves
        alldiff = "c Only one amazon per column:" :
          [ show x ++ "!=" ++ show k ++ " " ++ show y ++ "!=" ++ show k ++ " 0"
            | x <- [1..n], y <- [1..n], x < y, k <- [0..m] ]
        diagonal = "c only one per diagonal:" :
          [ show x ++ "!=" ++ show k ++ " " ++ show y ++ "!=" ++ show (k+(y-x)) ++ " 0"
            | x <- [1..n], k <- [0..m], y <- [(x+1)..n], k+(y-x) `elem` [0..m] ]
          ++
          [ show x ++ "!=" ++ show k ++ " " ++ show y ++ "!=" ++ show (k-(y-x)) ++ " 0"
            | x <- [1..n], k <- [0..m], y <- [(x+1)..n], k-(y-x) `elem` [0..m] ]
        knightmoves = "c attack relation of knights: " :
          [ show x ++ "!=" ++ show k ++ " " ++ show (x+1) ++ "!=" ++ show (k+2) ++ " 0"
            | x <- [1..n], k <- [0..m], x+1 `elem` [1..n], k+2 `elem` [0..m] ]
          ++
          [ show x ++ "!=" ++ show k ++ " " ++ show (x+1) ++ "!=" ++ show (k-2) ++ " 0"
            | x <- [1..n], k <- [0..m], x+1 `elem` [1..n], k-2 `elem` [0..m] ]
          ++
          [ show x ++ "!=" ++ show k ++ " " ++ show (x+2) ++ "!=" ++ show (k+1) ++ " 0"
            | x <- [1..n], k <- [0..m], x+2 `elem` [1..n], k+1 `elem` [0..m] ]
          ++
          [ show x ++ "!=" ++ show k ++ " " ++ show (x+2) ++ "!=" ++ show (k-1) ++ " 0"
            | x <- [1..n], k <- [0..m], x+2 `elem` [1..n], k-1 `elem` [0..m] ]
    _ -> error $ "Unknown arguments: " ++ show args
