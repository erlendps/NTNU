% Task 5
% procedure that calculates area, diameter and circumference and prints the result
declare Circle Pi

Pi = 355.0 / 113.0 % declate Pi

proc {Circle R} Rfloat A D C in
  % need to radius to float
  if {Int.is R} then
    Rfloat = {Int.toFloat R}
  else
    Rfloat = R
  end
  A = Pi * {Pow Rfloat 2.0} % calculate area
  D = 2.0 * Rfloat % calculate diameter
  C = Pi * D % calculate cicrumference
  {System.showInfo "R = "#R#", A = "#A#", D = "#D#", C = "#C}
end

{Circle 6}