/* Small instruction-stream fixtures for evaluating broad SFPU optimizations.
   These use target builtins directly so the final assembly can be inspected
   without pulling in TT-Metal headers.  They are not numerical tests.  */

namespace latency_mockup {

void serial_horner_chains ()
{
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto x = __builtin_rvtt_sfpreadlreg (2);
  auto c = __builtin_rvtt_sfpreadlreg (10);

  a = __builtin_rvtt_sfpmad (a, x, c, 0);
  a = __builtin_rvtt_sfpmad (a, x, c, 0);
  a = __builtin_rvtt_sfpmad (a, x, c, 0);
  a = __builtin_rvtt_sfpmad (a, x, c, 0);

  b = __builtin_rvtt_sfpmad (b, x, c, 0);
  b = __builtin_rvtt_sfpmad (b, x, c, 0);
  b = __builtin_rvtt_sfpmad (b, x, c, 0);
  b = __builtin_rvtt_sfpmad (b, x, c, 0);

  __builtin_rvtt_sfpwritelreg (a, 0);
  __builtin_rvtt_sfpwritelreg (b, 1);
}

void interleaved_horner_chains ()
{
  auto a = __builtin_rvtt_sfpreadlreg (0);
  auto b = __builtin_rvtt_sfpreadlreg (1);
  auto x = __builtin_rvtt_sfpreadlreg (2);
  auto c = __builtin_rvtt_sfpreadlreg (10);

  a = __builtin_rvtt_sfpmad (a, x, c, 0);
  b = __builtin_rvtt_sfpmad (b, x, c, 0);
  a = __builtin_rvtt_sfpmad (a, x, c, 0);
  b = __builtin_rvtt_sfpmad (b, x, c, 0);
  a = __builtin_rvtt_sfpmad (a, x, c, 0);
  b = __builtin_rvtt_sfpmad (b, x, c, 0);
  a = __builtin_rvtt_sfpmad (a, x, c, 0);
  b = __builtin_rvtt_sfpmad (b, x, c, 0);

  __builtin_rvtt_sfpwritelreg (a, 0);
  __builtin_rvtt_sfpwritelreg (b, 1);
}

} // namespace latency_mockup
