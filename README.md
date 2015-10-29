# WATCoin
Code from our Software Engineering 101 Lab 2 on the Tiva Launchpad with an Orbit Booster Pack.

Making a BitCoin Clone for Dummies: https://docs.google.com/file/d/0B82OOXRXzyeaaVVCeXByZ2JnR1U/edit?pli=1


How to run watcoind
---
You probably want Linux. If you don't have that, good luck, have fun, and update the guide.

- On arch: `pacman -Syu base-devel boost db`.  `libboost-all-dev` and `libdb5.1++-dev` should fufill the requirements on Ubuntu.
- go into coin folder
- cd `src`
- `ldd watcoind`
- Do you see any (not found)? If not, you're good to go. Skip to the end.
- If you do, recompile time.
- `make -f makefile.unix USE_UPNP=-`
- Let it build. Warnings are normal, errors are not. Try to resolve dependancies if they come up, and edit this guide with the solns.
- **finished**?
- Time to connect to the network.
- `./watcoind -irc`
- This will quit immediately and tell you to create a file with some content. Do that.
- `./watcoind -irc &`
- This will connect you to the network. Run a few commands to confirm:
- `./watcoind getpeerinfo` should have some info in there. Probably only one peer at this point (seed node, on my server)
- `./watcoind getinfo` should give you some basic stats.

Time to mine. How do you do this?
You have a choice here. You can start CPU mining (massively inefficient, but works) with `./watcoind setgenerate true` and append the number of cores you have.

For GPU mining, it depends on your GPU.

I can't speak for solutions for AMD GPUs, but cudaminer works great for NVIDIA.
Invoke it:

`cudaminer -d 0 -i 1 -l auto -o localhost:55883 -u watcoinrpc -p watpass`

replacing watcoinrpc and watpass with the username/password from `~/.watcoin/watcoin.conf`

Leave that running for a while. Every `yay!!!` you see is a new block that you successfully mined. Each one earns you 94 coins.

You'll likely wind up with ludicrous numbers of coins with a reasonably powerful GPU - a 750ti mined me ~30000 coins in 40 minutes. Keep at it, or the difficulty will never level off.

