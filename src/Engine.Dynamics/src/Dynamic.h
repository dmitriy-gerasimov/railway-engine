#pragma once

#include "Coupling.h"

class RailedVehicle;

class Dynamic
{
public:
	// усколение свободного падения
	static double const G;

	static double const Epsilon;

	Dynamic(Dynamic const& a_dynamics) = delete;
	auto operator= (Dynamic const& a_dynamics) -> Dynamic& = delete;

	Dynamic(Dynamic&& a_dynamics) = default;
	auto operator= (Dynamic&& a_dynamics)->Dynamic& = default;

	Dynamic(double a_mass, double a_length);

	auto getFrontCoupling() const -> Coupling const*;

	auto getFrontCoupling()->Coupling*;
	
	auto getBackCoupling() const -> Coupling const*;

	auto getBackCoupling() -> Coupling*;

	auto getOtherCoupling(Coupling const* a_coupling) const -> Coupling const*;

	auto getMass() const -> double;

	auto getLength() const -> double;
	
	auto getAcceleration() const -> double;

	auto getVelocity() const -> double;

	auto getVelocityKmPH() const -> double;

	auto getOffset() const -> double;

	auto setMass(double a_mass) -> void;

	auto getTractionForce() const -> double;

	auto setTractionForce(double a_force) -> void;

	auto getBrakeForce() const -> double;

	auto setBrakeForce(double a_force) -> void;

	auto setCouplingParameters(double k, double r, double freeWheelAmount) -> void;

	/**
	 * \brief Получить уклон
	 * \return 
	 */
	auto getGradient() const -> double;

	/**
	 * \brief Установить уклон
	 * \param a_gradient уклон (тангенс угла)
	 */
	auto setGradient(double a_gradient) -> void;
	
	auto update(double a_deltaSeconds) -> void;

	auto disconnectFrontCoupling() -> void;

	auto disconnectBackCoupling() -> void;

	/**
	 * \brief Получить коэффициент для силы трения
	 * \return 
	 */
	auto getFrictionForceFactor() const -> double;

	/**
	 * \brief Установка коэффициента для силы трения
	 * \param a_factor Коэффициент
	 */
	auto setFrictionForceFactor(double a_factor) -> void;

	auto getDistance() const -> double;

private:
	auto getFrictionForce() const -> double;

	// передняя автосцепка
	Coupling frontCoupling;

	// задняя автосцепка
	Coupling backCoupling;

	// масса, кг
	double mass;

	// ускорение, м/с2
	double acceleration;

	// скорость (> 0.0 - едем вперёд, иначе - назад), м/с
	double velocity;

	double previousVelocity;

	// смещение за один кадр (> 0.0 - едем вперёд, иначе - назад), м
	double offset;
		
	// сила тяги (> 0.0 - если сила тянет вагон вперёд), Н
	double tractionForce;

	// сила торможения (знак противоположен скорости), Н
	double brakeForce;

	// уклон, м/км (если > 0.0, то подъём)
	double gradient;

	// сила трения (знак противоположен скорости), Н
	double frictionForce;

	// коэффициент для корректировки силы трения
	double frictionForceFactor;

	// растояние, пройденное с начала движения
	double distance;

	// длина вагона
	double length;
};