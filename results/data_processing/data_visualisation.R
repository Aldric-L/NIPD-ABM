data <- read.csv("data.csv")

library(ggplot2)

ggplot(data, aes(iteration, cooperator_rate)) +
  geom_point(size = 0.5)
