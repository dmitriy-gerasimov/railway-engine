#pragma once

#include "Engine.Graph/src/Vector3D.h"

class Dynamic;

class Coupling
{
public:
	
	Coupling(Coupling const& a_coupling) = delete;
	auto operator= (Coupling const& a_coupling) -> Coupling& = delete;

	Coupling(Coupling&& a_coupling) = delete;
	auto operator= (Coupling&& a_coupling)->Coupling & = delete;

	Coupling(bool a_isFront);

	~Coupling();

	auto getConnectedCoupling() const -> Coupling*;

	auto setConnectedCoupling(Coupling* a_coupling) -> void;

	auto getK() const -> double;

	auto setK(double a_k) -> void;

	auto getR() const -> double;

	auto setR(double a_r) -> void;

	auto getIsOpened() const -> bool;

	auto setIsOpened(bool a_isOpened) -> void;

	auto getForce() const -> double;

	auto hasConnectedCoupling() const -> bool;

	auto getWorldLocation() const -> Vector3D;
	
	auto setWorldLocation(Vector3D a_location) -> void;

	auto getVelocity() const -> double;

	auto setVelocity(double a_velocity) -> void;

	auto getMass() -> const double;

	auto setMass(double a_mass) -> void;

	auto getIsFront() const -> bool;

	auto getIsBack() const -> bool;

	auto update(double a_deltaSeconds) -> void;

	auto getDistanceToConnectedCoupling() const -> double;

	auto setCouplingParameters(double k, double r, double freeWheelAmount) -> void;

private:	
	static double const Epsilon;

	Coupling* connectedCoupling;

	// расстояние между сцепками в момент сцепления
	double initialDistanceToConnectedCoupling;

	// коэффициент, влияющий на силу между сцепками в зависимости от расстояния между ними
	double k;

	// коэффициент, влияющий на силу между сцепками в зависимости от разности скоростей
	double r;

	// true - если сцепка открыта (может расцепиться)
	bool isOpened;

	// 3D координата
	Vector3D worldLocation;

	// 3D координата соседней автосцепки
	Vector3D connectedWorldLocation;

	// скорость вагона
	double velocity;

	double mass;

	// true - если автосцепка передняя, иначе - задняя
	bool isFront;

	// сила, действующая на автосцепку
	double force;

	// расстояние до соседней автосцепки
	double distanceToConnectedCoupling;

	// скорость соседнего вагона
	double neighborVelocity;

	// велечина свободного хода сцепки (есои расстояние между сцепками находится в интервале [-zeroThreshold; zeroThreshold],
	// то усилие на сцепках не возникает)
	double zeroThreshold;
};
