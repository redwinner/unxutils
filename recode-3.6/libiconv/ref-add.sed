/^# Packages using this file: / {
  s/# Packages using this file://
  ta
  :a
  s/ recode / recode /
  tb
  s/ $/ recode /
  :b
  s/^/# Packages using this file:/
}
