/^# Packages using this file: / {
  s/# Packages using this file://
  ta
  :a
  s/ textutils / textutils /
  tb
  s/ $/ textutils /
  :b
  s/^/# Packages using this file:/
}
