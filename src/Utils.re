let round = (nbr, decimals) => {
  let pow = float_of_int(Js.Math.pow_int(~base=10, ~exp=decimals));

  Js.Math.round(nbr *. pow) /. pow;
};
