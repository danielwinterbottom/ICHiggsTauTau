import matplotlib.pyplot as plt

gU = [0.5,1,1.5,2,2.5,3,3.5,4]
gU_XS = [0.0000358472469013,0.000557532756897,0.00282825160935,0.00910846773291,0.0224072784886,0.0458742391043,0.0854813581568,0.145746280488]

beta_L_23 = [0,0.1,0.2,0.3,0.4]
beta_L_23_XS = [0.0458742391043,0.0466979920032,0.0503598935426,0.055097472411,0.063347997501]

beta_L_32 = [0,0.1,0.2,0.3,0.4]
beta_L_32_XS = [0.0458742391043,0.0451334613155,0.0460521857257,0.0459442181127,0.0464570642743]

beta_Rd_33 = [0,0.5,1,1.5,2,2.5,3,3.5,4]
beta_Rd_33_XS = [0.0055188445122,0.0145876241504,0.0458742391043,0.11317605058,0.234709594162,0.445226445422,0.775617338064,1.2726182527,2.00059988005]

plt.figure(1)
plt.plot(gU,gU_XS)
plt.xlabel(r"$g_U$",size=15)
plt.ylabel("XS")
plt.text(2.2, 0.15, r'$\beta_{R}^{b\tau}=1, \beta_{L}^{b\tau}=1, \beta_{L}^{b\mu}=0, \beta_{L}^{s\tau}=0$',size=15)
plt.savefig("gU_XS.pdf")

plt.figure(2)
plt.plot(beta_L_23,beta_L_23_XS)
plt.xlabel(r"$\beta_L^{s\tau}$",size=15)
plt.ylabel("XS")
plt.text(0.2, 0.0635, r'$g_{U}=3, \beta_{R}^{b\tau}=1, \beta_{L}^{b\tau}=1, \beta_{L}^{b\mu}=0$',size=15)
plt.savefig("beta_L_23_XS.pdf")

plt.figure(3)
plt.plot(beta_L_32,beta_L_32_XS)
plt.xlabel(r"$\beta_L^{b\mu}$",size=15)
plt.ylabel("XS")
plt.text(0.2, 0.0465, r'$g_{U}=3, \beta_{R}^{b\tau}=1, \beta_{L}^{b\tau}=1, \beta_{L}^{s\tau}=0$',size=15)
plt.savefig("beta_L_32_XS.pdf")

plt.figure(4)
plt.plot(beta_Rd_33,beta_Rd_33_XS)
plt.xlabel(r"$\beta_R^{b\tau}$",size=15)
plt.ylabel("XS")
plt.text(2.2, 2.3, r'$g_{U}=3, \beta_{L}^{b\tau}=1, \beta_{L}^{b\mu}=0, \beta_{L}^{s\tau}=0$',size=15)
plt.savefig("beta_Rd_33_XS.pdf")

