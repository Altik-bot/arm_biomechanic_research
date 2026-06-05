float moveToPoint(int x,int y,int z,int upper_arm,int forearm){
	float l1 = sqrt(x*x + y*y);
	float alpha = atan2(y,x);
	float h = sqrt(z*z + l1*l1);
	float omega = acos((h*h-upper_arm*upper_arm-forearm*forearm)/(2*upper_arm*forearm));
	float beta = atan2(z,l1);
	float gamma = atan2((forearm*sin(omega)),(upper_arm+forearm*cos(omega)));
	float phi = beta - gamma;
return alpha,phi,omega;
}
