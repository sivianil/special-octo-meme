# MSE=mean squared error=sum((y_pred-y_actual)^2)) 
# /N as a function of degree of model. N-number of data points.
aic  = c()
mse = c()
for (i in 1:15){
  model <- lm(mpg~poly(disp,i), data=mtcars)
  pred <-predict(model)
  aic <- c(aic, AIC(model))
  mse <- c(mse, sum((pred-y)^2)/length(y))
}

plot(c(1:15), mse, xlab ="degree", ylab="MSE", ylim=c(min(mse)*0.9,max(mse)*1.1),
     main="MSE with y=mpg and x=disp in mtcars data")

print("The best model based MSE is the polynomial of higher degree")


# Plot AIC=AIC(degree).

plot(c(1:15), aic, xlab ="degree", ylab="AIC", ylim=c(min(aic)*0.9, max(aic)*1.1),
     main="AIC with y=mpg and x=disp in mtcars data")

print("The most reasonable AIC is for the polynomial degree 3")

# Set up a Leave-One-Out (LOO) cross-validation and determine the relationship
# between AIC=AIC(degree of model) and MSE (residual sum of squares) of prediction
# in a LOO-setting (MSE=MSE(degree)), What is the relationship?

library(boot)

aic_values_loo <- c()
cv_loo <- c()
n<- dim(mtcars)[1]
for (i in 1:4){
  glm.fit <- glm(mpg~poly(disp,i), data=mtcars)
  cv.err= cv.glm(mtcars, glm.fit)$delta[1]
  aic_values_loo <- c(aic_values_loo, AIC(glm.fit))
  cv_loo <- c(cv_loo, cv.err)
}
plot(aic_values_loo, cv_loo, xlab ="AIC", ylab="cross validation error", 
     xlim=c(min(aic_values_loo)*0.9, max(aic_values_loo)*1.1),
     ylim=c(min(cv_loo)*0.9, max(cv_loo)*1.1),
     main="LOO: AIC and CV error with y=mpg and x=disp in mtcars data")

print("It seems that the relationship between AIC and cv error is proportional")