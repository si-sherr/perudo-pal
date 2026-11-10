# Perudo Pal
## Introduction
### About Perudo Pal
Perudo Pal is a simple CLI tool written in C that can calculate the statistical probability of a Perudo bids winning or losing within a given round of Perudo.
### What is Perudo?
Perudo, also known as [Dudo](https://en.wikipedia.org/wiki/Dudo) or "Liar's Dice", is a popular dice game based on bluffing and chance. Players make increasingly high "bids" on the total number of dice, placed under cups, that are showing a certain number on the table. The player who loses a round loses one of their dice. The last player remaining with dice is the winner.

### What is Perudo Pal Useful For?
While Perudo is a game both of bluffing *and* probability. Perudo Pal is a tool for the latter. It takes into account the game parameters known by the user/player, and calculates the probability that certain bids will win or lose. Use Perudo Pal when you want to know the probability of a particular bid winning/losing, or you want to know which bid you can make that gives you the best odds.

I made this tool mainly as a personal curiosity, and I am not suggesting that using it will turn you into an unstoppable Perudo player. t remains to be seen how well a player would get on if they were to use only this tool to play a Perudo game.

## Getting Started
### Requirements

- A Linux system
- A C compiler
- CMake

### Installation
Helper scripts used for development are contained within the `scripts/` directory. `build.sh` configures and builds the CMake project. `install.sh` copies the built executable to `~/.local/bin/` so it is in your path.

### Development
The `build.sh` script also builds a [GoogleTest](https://github.com/google/googletest) unit test project. Run the tests with `test.sh`. This project uses the [Fake Function Framework](https://github.com/meekrosoft/fff) for fakes/mocks. `format.sh` applies formatting to C, Cpp and CMake files. It requires clang-format and cmake-format installed.

The C standard used for this project is C23.

## Usage Guide
### Running the App
Perudo Pal requires information on the current state of the Perudo round to calculate bid probabilities. This includes the following:

- How may dice are currently in play (including your own)
- Which dice are currently in the players "hand" (under their cup)
- The current bid
- Whether Palifico rules are currently in play

For example, if there are **30 total dice** in play, the current bid is **9 6s** ("nine sixes"), and the player has the following dice under their cup: **1, 4, 4, 6, 4**, the equivalent `ppal` command would be as follows:

```bash
ppal --total-dice 30 --bid 9,6 --hand 1,4,4,6,4
```

Which would yield the following output:

```bash
Bid:         Quantity: Value: P(win): P(exact): P(lose):
Current              9      6   0.778     0.149    0.222
Next Best           10      4   0.888     0.110    0.112
```

The `--bid` argument should always be provided in the format `q,v`, where `q` and `v` represent the bid **quantity** and **value** respectively.

Values provided for the `--bid` and `--hand` arguments should be **comma-separated** and contain **no spaces**.


### Interpreting the Results

Information on two bids is output by ppal: The *Current* Bid and the *Next Best* Bid. The Next Best Bid is the bid with the **highest probability of winning** that is **legally playable** from the current bid.

For each bid, following information is displayed:

- P(win): The probability that the bid is true. In the case of the previous example, this is the probability that there are **9 or more** sixes under all cups.
- P(exact): The probability that bid is exactly satisfied. (In our example, the probability that there are **exactly 9** sixes under all cups.)
- P(lose): The probability that the bid is false and would cause the player holding that bid to lose the round. (In our example the probability that there are **8 or fewer** sixes under all cups.)

It is worth noting that P(exact) is included in P(win), so P(win) + P(lose) = 1.

When it is your turn to bid, ppal can tell you the statistically best action. As a player, you have two options:

- Make a new bid
- Challenge the current bid

When you challenge the current bid, the chance that you win the challenge is P(lose) for that bid. If someone challenges the next bid you make, the chance that you win the challenge is P(win) for that bid. Your best option is therefore summarised by the following pseudocode:

```C
if (p_next_bid_win > p_current_bid_lose):
    make_next_bid();
else:
    challenge_current_bid();
```

Of course, this is all made slightly more complicated if you take into account tha fact that the player after you may not always challenge your bid but instead and make a bid of their own, which would increase your overall win chances if you take the next bid, but this is out of scope for a purely statistical tool.

### Notes
Bidding rules are different when a Palifico round is active. Use the `--palifico` or `-p` flags to activate these rules.

To print version information, use the `--version` or `-v` flags. To print usage information, use the `--help` or `-h` flags.

Perudo Pal can also handle non-standard dice. Use the `--faces` or `-f` flags to specify if your dice are not 6-sided.

## License
This software is licensed under the [MIT license](./LICENSE.txt).
